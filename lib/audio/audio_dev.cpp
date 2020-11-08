#include "audio_dev.h"
#include <iostream>
#include <limits>
#include <alsa/asoundlib.h>

DynamicSounds::AudioDevice::AudioDevice(string dev_name, unsigned int sample_rate, unsigned int buff_size, unsigned int period_size, SoundSource *sound_source){
    this->dev_name = dev_name;
    this->sample_rate = sample_rate;
    this->period_size = (snd_pcm_uframes_t)period_size;
    this->buff_size = (snd_pcm_uframes_t)buff_size; //snd_pcm_uframes_t is just a quantity, I think...
    this->sound_source = sound_source;

    if(sound_source->GetSampleRate() != this->sample_rate){
        std::printf("Warning! Sample rate of sound source is not the same as AudioDevice's!\n");
        std::printf("Overriding source's sample rate of %d with AudioDevice's sample rate of %d\n", sound_source->GetSampleRate(), this->sample_rate);
        sound_source->SetSampleRate(this->sample_rate);
    }
};

DynamicSounds::AudioDevice::AudioDevice(string dev_name, unsigned int sample_rate, unsigned int buff_size, unsigned int period_size){
    this->dev_name = dev_name;
    this->sample_rate = sample_rate;
    this->period_size = (snd_pcm_uframes_t)period_size;
    this->buff_size = (snd_pcm_uframes_t)buff_size; //snd_pcm_uframes_t is just a quantity, I think...
};

void DynamicSounds::AudioDevice::alsa_callback_director(snd_async_handler_t *handler){

    AudioDevice *device = (DynamicSounds::AudioDevice*)snd_async_handler_get_callback_private(handler);

    if(device != NULL){
        device->audio_cb(handler);
    }
}

void DynamicSounds::AudioDevice::SetSoundSource(SoundSource *source){
    this->sound_source = source; 
}

void DynamicSounds::AudioDevice::Start(){
    snd_pcm_start(this->pcm_handle);
}

void DynamicSounds::AudioDevice::Stop(){
    snd_pcm_drop(this->pcm_handle); //Drops remaining frames, does not play the rest of the buffer
}

void DynamicSounds::AudioDevice::audio_cb(snd_async_handler_t *handler){
    std::cout << "Audio is calling back" << std::endl;

    snd_pcm_sframes_t avail;
    int err;

    double _tmpArray[this->logical_buff_size];
    this->sound_source->GenerateFrames(_tmpArray, this->logical_buff_size);

    this->buffer = new unsigned short[this->logical_buff_size]();

    for (size_t i = 0; i < this->logical_buff_size; i++)
    {
        this->buffer[i] = (short)( ( (1 + _tmpArray[i]) * 65535 ) /2 ); //maximum for unsigned short (16-bit)
    }

    avail = snd_pcm_avail_update(this->pcm_handle);
    while (avail >= period_size) {
        snd_pcm_writei(pcm_handle, this->buffer, period_size);
        avail = snd_pcm_avail_update(this->pcm_handle);
    }
};

bool DynamicSounds::AudioDevice::Initialize(){

    unsigned short channels = 2;

    //I am ashamed, this is all very copy paste. I do not fully understand ALSA right now, so this is an OK start to get noise to play.

    unsigned int pcm, tmp, dir;

    if (pcm = snd_pcm_open(&this->pcm_handle, this->dev_name.c_str(),SND_PCM_STREAM_PLAYBACK, 0) < 0){
        printf("ERROR: Can't open \"%s\" PCM device. %s\n", this->dev_name.c_str(), snd_strerror(pcm)); 
        return false;
    }

    /* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&this->pcm_params); //Allocate memory for the parameter structure
    snd_pcm_hw_params_any(this->pcm_handle, this->pcm_params);

    /* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, this->pcm_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
        std::printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm)); 
        return false;
    }

    if (pcm = snd_pcm_hw_params_set_format(pcm_handle, this->pcm_params, SND_PCM_FORMAT_S16_LE) < 0) {
        std::printf("ERROR: Can't set format. %s\n", snd_strerror(pcm)); 
        return false;
    }
        

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, this->pcm_params, channels) < 0) {
        std::printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm)); 
        return false;
    }
		

    
    unsigned int last_sample_rate = this->sample_rate;
    //Watch out! this function can change your supplied sample rate to something different if the device needs to!
	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, this->pcm_params, &this->sample_rate, 0) < 0) {
        std::printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm)); 
        return false;
    }
		
    if(this->sample_rate != last_sample_rate){
        std::printf("Sample rate changed to %d\n", this->sample_rate);
        this->sound_source->SetSampleRate(this->sample_rate);
    }

    //Watch out! this function can change your supplied buffer size to something different if the device needs to!
    snd_pcm_hw_params_set_buffer_size_near(this->pcm_handle, this->pcm_params, &this->buff_size);

    //Watch out! this function can change your supplied period size to something different if the device needs to!
    //Additionally, period size is really only conceptually applicable in non-interleaved signals, but this still should be processed in a general case.
    snd_pcm_hw_params_set_period_size_near(this->pcm_handle, this->pcm_params, &this->period_size, NULL);

    /*
        Periods!

        The period reflects how big a chunk of data for one channel will be. (FOR NON-INTERLEAVED DATA) 
        The buffer size reflects an absoulte capacity of data to deal with at one time. 

        You can consider the amount of periods in your buffer as buffer_size/period_size = n_periods.
        For example, a buffer with a data size of 1024 bytes that contains periods of 64 bytes, will only hold 16 periods.
        1024/64 = 16

        To know how much data is in one sample, interleaved or not, refer to 'snd_pcm_hw_params_set_format' called earlier. 
        For example, the constant SND_PCM_FORMAT_S16_LE means our samples are 16 bits wide (two bytes) and are 'L'ittle 'E'ndian

        So in a case where we are using non-interleaved data, lets say our periods are 64 bytes long. Using 16 bit (2 byte) samples, our periods
        will contain 64/2 samples, or 32 samples for a single channel. 
        |
        Again, each period represents a channel. 

        For example, consider this our entire buffer: LL LL LL LL RR RR RR RR LL LL LL LL RR RR RR RR
        |
        This example reflects 2 bytes (16 bits) per sample, and has a period size of 4 samples per channel, meaning 8 bytes per period.
        Our buffer size will be 2(bytes per sample) * 4(samples)  * 2(periods per channel) * 2(channels) = 32 bytes per buffer
        A more direct variable formula: buffer_size = sample_size * samples_per_period * periods_per_channel * channels
        This formula can be manipulated to yeild whatever other variable you need, given you have the others. This formula also assumes 
        that your buffer type is a standard 8-bit data-type. Should you use a different data-type to compensate for your sample size,
        you must adjust your buffer size accordingly. 

        logical_buffer_size = buffer_size/bytes_per_data_type 
        logical_buffer_size = buffer_size/sizeof(T)



        Interleaved is different!

        For INTERLEAVED data, periods do not conceptually exist, and the data is sent in channels one after the other.
        For example, consider this our entire buffer:  LL RR LL RR LL RR LL RR
        |
        This example reflects 2 bytes (16 bits) per sample (Each sample is a group of Ls or Rs), and 2 channels here. We have 8 samples (4 per channel) meaning 
        our buffer size is 4(samples) * 2(bytes per channel) * 2(channels) = 16 bytes (not bits). 
        Here is a more direct formula: buffer_size = sample_size * samples_per_channel * channels
        This formula can be manipulated to yeild whatever other variable you need, given you have the others.
        Again, this assumes your buffer type is a standard 8-bit data-type. Read above if you desire to use a different type.


        Hopefully the insight is helpful. Thanks to this wonderful gem: https://alsa.opensrc.org/Asynchronous_Playback_(Howto)
    */

    /* Write parameters */
	if (pcm = snd_pcm_hw_params(this->pcm_handle, this->pcm_params) < 0){
        std::printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm)); 
        return false;
    }
		

    /* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(this->pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(this->pcm_handle)));

	snd_pcm_hw_params_get_channels(this->pcm_params, &tmp);
	printf("channels: %i ", tmp);
    this->channels = tmp;

    if (this->channels == 1)
		std::printf("(mono)\n");
	else if (this->channels == 2)
		std::printf("(stereo)\n");

    snd_pcm_hw_params_get_rate(this->pcm_params, &tmp, 0);
	printf("rate: %d bps\n", tmp);
    
    //snd_pcm_hw_params_free(this->pcm_params); //This does bad stuff right now...?

    /* Software params */
    snd_pcm_sw_params_malloc(&this->sw_params); //Allocate memory for the parameter structure
    snd_pcm_sw_params_current(this->pcm_handle, this->sw_params); //Get the current software parameters

    snd_pcm_sw_params_set_start_threshold(this->pcm_handle, this->sw_params, this->buff_size - period_size); //buff_size - period_size seems to be a good threshold for generating more data.
    snd_pcm_sw_params_set_avail_min(this->pcm_handle, this->sw_params, period_size);

    snd_pcm_sw_params(this->pcm_handle, this->sw_params);

    snd_pcm_sw_params_free(this->sw_params); //This does bad stuff right now...?

    /* Prepare to write! */
    snd_pcm_prepare(this->pcm_handle);

    /* Calculate logical buffer_size */
    this->logical_buff_size = buff_size/sizeof(short);

    /* Write an initial stream of data... */
    double _tmpArray[this->logical_buff_size];
    this->sound_source->GenerateFrames(_tmpArray, this->logical_buff_size);

    this->buffer = new unsigned short[this->logical_buff_size]();

    for (size_t i = 0; i < this->logical_buff_size; i++)
    {
        this->buffer[i] = (short)( ( (1 + _tmpArray[i]) * 65535 ) /2 ); //maximum for unsigned short (16-bit)
        
    }
    

    snd_pcm_writei(this->pcm_handle, this->buffer, 2 * period_size);

    //TODO: No no callback! Callback bad! It onlhy work with very loaw level devisays!
    int cb_return;
    if(cb_return = snd_async_add_pcm_handler(&this->pcm_callback, this->pcm_handle, AudioDevice::alsa_callback_director, this) < 0){
        std::printf("ERROR: Can't set callback, %s\n", snd_strerror(cb_return));
        return false;
    }

    return true;
    
}


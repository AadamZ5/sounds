#include "audio_dev.h"
#include <alsa/asoundlib.h>

DynamicSounds::AudioDevice::AudioDevice(string dev_name, unsigned int sample_rate, unsigned int buff_size){
    this->dev_name = dev_name;
    this->sample_rate = sample_rate;
    this->buff_size = (snd_pcm_uframes_t)buff_size;
};

void DynamicSounds::AudioDevice::audio_cb(snd_async_handler_t *handler){

}

bool DynamicSounds::AudioDevice::initialize(){

    unsigned short channels = 1;
    unsigned short seconds = 2;

    //I am ashamed, this is all very copy paste. I do not fully understand ALSA right now, so this is an OK start to get noise to play.

    unsigned int pcm, tmp, dir;

    if (pcm = snd_pcm_open(&this->pcm_handle, this->dev_name.c_str(),SND_PCM_STREAM_PLAYBACK, 0) < 0) 
		printf("ERROR: Can't open \"%s\" PCM device. %s\n", this->dev_name.c_str(), snd_strerror(pcm));

    /* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&this->pcm_params);

    snd_pcm_hw_params_any(this->pcm_handle, this->pcm_params);

    /* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, this->pcm_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
        printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_format(pcm_handle, this->pcm_params, SND_PCM_FORMAT_S16_LE) < 0) 
        printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, this->pcm_params, channels) < 0) 
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, this->pcm_params, &this->sample_rate, 0) < 0) 
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

    snd_pcm_hw_params_set_buffer_size_near(this->pcm_handle, this->pcm_params, &this->buff_size);

    snd_pcm_uframes_t period_size = 64;
    snd_pcm_hw_params_set_period_size_near(this->pcm_handle, this->pcm_params, &period_size, NULL);

    /* Write parameters */
	if (pcm = snd_pcm_hw_params(this->pcm_handle, this->pcm_params) < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

    /* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(this->pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(this->pcm_handle)));

	snd_pcm_hw_params_get_channels(this->pcm_params, &tmp);
	printf("channels: %i ", tmp);

    if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

    snd_pcm_hw_params_get_rate(this->pcm_params, &tmp, 0);
	printf("rate: %d bps\n", tmp);

	printf("seconds: %d\n", seconds);
    
    snd_pcm_hw_params_free(this->pcm_params);

    /* Software params */
    snd_pcm_sw_params_malloc(&this->sw_params);
    snd_pcm_sw_params_current (this->pcm_handle, this->sw_params);

    snd_pcm_sw_params_set_start_threshold(this->pcm_handle, this->sw_params, this->buff_size - period_size);
    snd_pcm_sw_params_set_avail_min(this->pcm_handle, this->sw_params, period_size);

    snd_pcm_sw_params(this->pcm_handle, this->sw_params);

    snd_pcm_sw_params_free(this->sw_params);

    /* Prepare to write! */
    snd_pcm_prepare(this->pcm_handle);

    /* Write initial stream of data... */

    unsigned int _tmp[this->buff_size];

    for (size_t i = 0; i < this->buff_size; i++)
    {
        _tmp[i] = 0;
    }
    

    snd_pcm_writei(pcm_handle, _tmp, 2 * period_size);

    snd_async_add_pcm_handler(&this->pcm_callback, this->pcm_handle, audio_cb, NULL);
    //TODO: FINISH
}


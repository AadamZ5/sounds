#include <iostream>
#include <alsa/asoundlib.h>
#include "../sounds/sound.h"

using namespace std;

namespace DynamicSounds{

    class AudioDevice{
        
        public:
            AudioDevice(string dev_name, unsigned int desired_sample_rate, unsigned int buff_size, unsigned int period_size);
            AudioDevice(string dev_name, unsigned int desired_sample_rate, unsigned int buff_size, unsigned int period_size, DynamicSounds::SoundSource *sound_source);
            ~AudioDevice();

            void SetSoundSource(DynamicSounds::SoundSource *source);
            bool Initialize();
            void Start();
            void Stop();

        private:
            static void alsa_callback_director(snd_async_handler_t *handler);

            void audio_cb(snd_async_handler_t *handler);

            DynamicSounds::SoundSource *sound_source;

            string dev_name;
            unsigned int sample_rate;
            snd_pcm_uframes_t buff_size;
            unsigned int logical_buff_size;
            snd_pcm_uframes_t period_size;
            unsigned int channels;

            unsigned short *buffer;


            snd_pcm_t *pcm_handle;
            snd_pcm_hw_params_t *pcm_params;
            snd_pcm_uframes_t frames;

            snd_pcm_sw_params_t *sw_params;

            snd_async_handler_t *pcm_callback;

        
    };

};
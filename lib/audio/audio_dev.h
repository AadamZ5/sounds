#include <iostream>
#include <alsa/asoundlib.h>

using namespace std;

namespace DynamicSounds{

    class AudioDevice{
        
        public:
            AudioDevice(string dev_name, unsigned int sample_rate, unsigned int buff_size);
            ~AudioDevice();

            bool initialize();

        private:
            void audio_cb(snd_async_handler_t *handler);

            string dev_name;
            unsigned int sample_rate;
            snd_pcm_uframes_t buff_size;

            snd_pcm_t *pcm_handle;
            snd_pcm_hw_params_t *pcm_params;
            snd_pcm_uframes_t frames;

            snd_pcm_sw_params_t *sw_params;

            snd_async_handler_t *pcm_callback;

        
    };

};
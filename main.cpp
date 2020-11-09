#include <iostream>
#include <alsa/asoundlib.h>
#include "lib/effects/effect.h"
#include "lib/sounds/sine.h"
#include "lib/audio/audio_dev.h"

int main (int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    DynamicSounds::StandardFunction();

    DynamicSounds::SineWave *sw = new DynamicSounds::SineWave(440, 44100);

    DynamicSounds::AudioDevice *ad = new DynamicSounds::AudioDevice(argv[1], 44100, 4096, 512, sw);
    
    if(ad->Initialize()){
        ad->Start();

        for (size_t i = 0; i < 100; i++)
        {
            sleep(1);
        }

        ad->Stop();
    }else{
        std::printf("Error while initializing audio device.\n");
    }
    exit(0);
}


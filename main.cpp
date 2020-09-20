#include <iostream>
#include <alsa/asoundlib.h>
#include "lib/effects/effect.h"
#include "lib/sound-maker/sgen.h"
#include "lib/audio/audio_dev.h"

using namespace std;

int main (int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    DynamicSounds::Wrapper::StandardFunction();

    DynamicSounds::SineWave *sw = new DynamicSounds::SineWave(440, 44100);

    double buffer[1024];

    //GenFrames!
    sw->GenerateFrames(buffer, 1024);

    DynamicSounds::AudioDevice *ad = new DynamicSounds::AudioDevice("default", 44100, 1024);
    ad->initialize();
    exit(0);
}


#include <iostream>
#include <alsa/asoundlib.h>
#include "lib/effects/effect.h"
#include "lib/sound-maker/sgen.h"

using namespace std;

int main (int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    DynamicSounds::Wrapper::StandardFunction();

    DynamicSounds::SineWave *sw = new DynamicSounds::SineWave(440, 44100);

    double buffer[44100];

    //GenFrames!
    sw->GenerateFrames(buffer, 44100);

    for (size_t i = 0; i < 44100; i++)
    {
        cout << "[" << i << "] " << buffer[i] << endl;
    }

    
    
    exit(0);
}


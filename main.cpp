#include <iostream>
#include <alsa/asoundlib.h>
#include "lib/effects/effect.h"
#include "lib/sound-maker/sgen.h"

using namespace std;

long int buf[1024];

int main (int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    DynamicSounds::Wrapper::StandardFunction();

    DynamicSounds::SineWave *sw = new DynamicSounds::SineWave(440, 44100);

    double buffer[1024];

    //GenFrames!
    sw->GenerateFrames(buffer, 1024);

    for (size_t i = 0; i < 1024; i++)
    {
        cout << "[" << i << "] " << buffer[i] << endl;
    }

    
    
    exit(0);
}


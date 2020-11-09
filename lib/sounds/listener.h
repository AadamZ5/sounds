#pragma once

#include "./sound.h"

/*
Listeners are used in SoundSpace classes, and inherit from the SoundSource class. Listeners provide a streamlined way to listen within a sound space. 

Adding a listener to a SoundSpace will allow you to simply use the listener to get sound data.
*/

namespace DynamicSounds{
    class Listener : DynamicSounds::SoundSource{
    private:

    public:
        Listener();
        ~Listener();
    };
}
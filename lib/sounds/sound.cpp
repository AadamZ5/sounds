#include "sound.h"

DynamicSounds::SoundSource::SoundSource(unsigned int sample_rate){
    this->sample_rate = sample_rate;
}

// unsigned int DynamicSounds::SoundSource::GetSampleRate(){
//     //This should not be called. Return 0u.
//     return 0u;
// }

// void DynamicSounds::SoundSource::SetSampleRate(unsigned int sample_rate){
//     //This should not be called. Do nothing.
// }
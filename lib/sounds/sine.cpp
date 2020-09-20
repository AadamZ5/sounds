#include "sine.h"
#include <limits>
#include <math.h>

DynamicSounds::SineWave::SineWave(double freq, unsigned int sample_rate) : DynamicSounds::SoundSource(sample_rate) {
    this->freq = freq;
    this->last_angle = 0;
    //this->sample_rate = sample_rate; //Base class takes care of this
    this->phase_shift = 0;
    this->last_angle = 0;
}

void DynamicSounds::SineWave::SetPhaseShift(double angle){
    this->phase_shift = angle;
}

void DynamicSounds::SineWave::SetAmplitude(double amplitude){
    this->amp = amplitude;
}

void DynamicSounds::SineWave::SetFrequency(double freq){
    this->freq = freq;
}

void DynamicSounds::SineWave::SetSampleRate(unsigned int sample_rate){
    this->sample_rate = sample_rate;
}

double DynamicSounds::SineWave::GetPhaseShift(){
    return this->phase_shift;
}

double DynamicSounds::SineWave::GetAmplitude(){
    return this->amp;
}

double DynamicSounds::SineWave::GetFrequency(){
    return this->freq;
}

unsigned int DynamicSounds::SineWave::GetSampleRate(){
    return this->sample_rate;
}

void DynamicSounds::SineWave::GenerateFrames(double buffer[], int buffer_size){

    double m_time;
    double time_step = 1.0/this->sample_rate;

    double angle;

    for (unsigned int sample = 0; sample < buffer_size; sample++)
    {
        if (m_time >= std::numeric_limits<float>::max()) {
            m_time = 0.0;
        }

        angle = (2 * M_PI * this->freq * m_time + this->phase_shift + this->last_angle);
        double value = sin(angle);
        buffer[sample] = value;

        m_time += time_step;
    }

    this->last_angle = angle;
}
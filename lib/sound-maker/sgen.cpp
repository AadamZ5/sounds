#include "sgen.h"
#include <limits>
#include <math.h>

DynamicSounds::SineWave::SineWave(double freq, int sample_rate){
    this->freq = freq;
    this->last_angle = 0;
    this->sample_rate = sample_rate;
    this->phase_shift = 0;
}

void DynamicSounds::SineWave::SetPhaseShift(double angle){
    this->phase_shift = angle;
}

void DynamicSounds::SineWave::GenerateFrames(double buffer[], int buffer_size){

    double m_frequency = this->freq;
    double m_phase = this->phase_shift;
    double m_time;
    double time_step = 1.0/this->sample_rate;

    for (unsigned int sample = 0; sample < buffer_size; sample++)
    {
        if (m_time >= std::numeric_limits<float>::max()) {
            m_time = 0.0;
        }

        double value = sin(2 * M_PI * m_frequency * m_time + m_phase);
        buffer[sample] = value;

        m_time += time_step;
    }
}
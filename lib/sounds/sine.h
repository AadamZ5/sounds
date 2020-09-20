#pragma once

#include "sound.h"

namespace DynamicSounds{

    class SineWave : public DynamicSounds::SoundSource {
        public:
            SineWave(double freq, unsigned int sample_rate);

            void GenerateFrames(double *, int);     //Virtual overrides
            unsigned int GetSampleRate();           //
            void SetSampleRate(unsigned int);       //
            void SetAmplitude(double);
            double GetAmplitude();
            void SetFrequency(double);
            double GetFrequency();
            void SetPhaseShift(double);
            double GetPhaseShift();

        private:
            double amp;
            double freq;
            double phase_shift;
            double last_angle;
    };

};


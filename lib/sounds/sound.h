#pragma once

namespace DynamicSounds{
    class SoundSource{
        public:
            SoundSource(unsigned int sample_rate);

            void virtual GenerateFrames(double *, int) = 0;
            unsigned int virtual GetSampleRate() = 0;
            void virtual SetSampleRate(unsigned int) = 0;

        protected:
            unsigned int sample_rate;
    };
};  
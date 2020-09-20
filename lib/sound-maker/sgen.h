
namespace DynamicSounds{

    class SineWave{
        public:
            SineWave(double freq, int sample_rate);

            void GenerateFrames(double *buffer, int buffer_size);
            void SetPhaseShift(double angle);

        private:
            int sample_rate;
            double freq;
            double phase_shift;
            double *last_angle;
    };

};


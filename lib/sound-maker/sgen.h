
namespace DynamicSounds{

    class SineWave{
        public:
            SineWave(double freq, int sample_rate);

            void GenerateFrames(double *, int);
            void SetAmplitude(double);
            double GetAmplitude();
            void SetFrequency(double);
            double GetFrequency();
            void SetPhaseShift(double);
            double GetPhaseShift();

        private:
            int sample_rate;
            double amp;
            double freq;
            double phase_shift;
            double *last_angle;
    };

};


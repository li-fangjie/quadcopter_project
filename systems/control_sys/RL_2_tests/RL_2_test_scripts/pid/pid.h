#ifndef __PID_H__
#define __PID_H__

class PID_CTRL
{
    private:
        float * input;
        float * p_output;
        float output;
        // float kP, kI, kD, kF;
        const float * k; // k[3] = {kP, kI, kD}
        float * sp;
        float resolution;
        float out_range[2];
        
        float i_error;
        float prev_error;
        
        bool state;

    public:
        void setup(float* n_input, float* n_sp, const float n_k[3]);

        void setup(float* n_input, float* n_output, float* n_sp, const float n_k[3]);

        void set_resolution(float reso);

        void set_output_range(float lower, float upper);

        void activate();

        void pause();

        void update();

        void post_process();

        void set_coef(const float n_k[3]);

        float* get_p_output();

        float* get_ext_p_output();

        float get_output();

        float get_kP();

        float get_kI();
        
        float get_kD();
};

#endif

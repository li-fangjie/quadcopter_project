#ifndef __PID_H__
#define __PID_H__

template <class T_I, class T_O, class T_S>
class PID_CTRL
{
    private:
        T_I * input;
        T_O * p_output;
        T_O output;
        // float kP, kI, kD, kF;
        const float * k; // k[3] = {kP, kI, kD}
        T_S * sp;
        float resolution;
        float out_range[2];
        
        float i_error;
        float prev_error;
        
        bool state;

    public:
        void setup(T_I* n_input, T_S* n_sp, const float n_k[3]);

        void setup(T_I* n_input, T_O* n_output, T_S* n_sp, const float n_k[3]);

        void set_resolution(float reso);

        void set_output_range(float lower, float upper);

        void activate();

        void pause();

        void update();

        void post_process();

        void set_coef(const float n_k[3]);

        T_O* get_p_output();

        T_O* get_ext_p_output();

        T_O get_output();

        float get_kP();

        float get_kI();
        
        float get_kD();
};

#endif

#include "pid_t.h"
#include "Arduino.h"

template <class T_I, class T_O, class T_S>
PID_CTRL<T_I, T_O, T_S>::PID_CTRL()
: input(NULL), p_output(NULL), output(0), k(NULL), 
  sp(NULL), resolution(0), i_error(0), prev_error(0), state(false)
{
  set_output_range(-255, 255);
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::setup(T_I* n_input, T_S* n_sp, const float n_k[3])
{
  input = n_input;
  sp = n_sp;
  k = n_k;
  p_output = NULL;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::setup(T_I* n_input, T_O* n_output, T_S* n_sp, const float n_k[3])
{
  input = n_input;
  sp = n_sp;
  k = n_k;
  p_output = n_output;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::set_resolution(float reso)
{
    resolution = reso;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::set_output_range(float lower, float upper)
{
    out_range[0] = lower;
    out_range[1] = upper;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::set_sp(T_S* n_sp)
{
    pause();
    sp = n_sp;
    activate();
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::set_input(T_I* n_input)
{
    pause();
    input = n_input;
    activate();
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::activate()
{
    state = true;
    i_error = 0;
    prev_error = 0;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::pause()
{
    state = false;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::update()
{
    if(!state) return;
    float error = *sp - *input;
    if (abs(error) < resolution){
        error = 0;
        // i_error = 0;
    }
    i_error += error;
    /*
    float p_term = k[0] * error;
    float i_term = k[1] * i_error;
    float d_term = k[2] * (error - prev_error);
    output = p_term + i_term + d_term;
    */
    output = 0;
    output += k[0] * error;
    output += k[1] * i_error;
    output += k[2] * (error - prev_error);
    post_process();
    if(p_output != NULL) *p_output = output;
    prev_error = error;
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::post_process()
{
    if(output < out_range[0]) output = out_range[0];
    if(output > out_range[1]) output = out_range[1];
}

template <class T_I, class T_O, class T_S>
void PID_CTRL<T_I, T_O, T_S>::set_coef(const float n_k[3])
{
    pause();
    k = n_k;
    activate();
}

template <class T_I, class T_O, class T_S>
T_O* PID_CTRL<T_I, T_O, T_S>::get_p_output()
{
    return &output;
}

template <class T_I, class T_O, class T_S>
T_O* PID_CTRL<T_I, T_O, T_S>::get_ext_p_output()
{
    return p_output;
}

template <class T_I, class T_O, class T_S>
T_O PID_CTRL<T_I, T_O, T_S>::get_output()
{
    return output;
}

template <class T_I, class T_O, class T_S>
float PID_CTRL<T_I, T_O, T_S>::get_kP()
{
    return k[0];
}

template <class T_I, class T_O, class T_S>
float PID_CTRL<T_I, T_O, T_S>::get_kI()
{
    return k[1];
}

template <class T_I, class T_O, class T_S>
float PID_CTRL<T_I, T_O, T_S>::get_kD()
{
    return k[2];
}

template <class T_I, class T_O, class T_S>
float PID_CTRL<T_I, T_O, T_S>::get_error()
{
    return prev_error;
}

template <class T_I, class T_O, class T_S>
float PID_CTRL<T_I, T_O, T_S>::get_i_error()
{
    return i_error;
}

template class PID_CTRL<float, float, float> ; // Outer pid
template class PID_CTRL<float, int16_t, float> ; // inner pid
template class PID_CTRL<int16_t, int16_t, float>;

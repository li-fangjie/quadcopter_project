#include "pid.h"
#include "Arduino.h"

void PID_CTRL::setup(float* n_input, float* n_sp, const float n_k[3])
{
    set_output_range(-255, 255);
    input = n_input;
    sp = n_sp;
    k = n_k;
    output = NULL;
}

void PID_CTRL::setup(float* n_input, float* n_output, float* n_sp, const float n_k[3])
{
    set_output_range(-255, 255);
    input = n_input;
    sp = n_sp;
    k = n_k;
    p_output = n_output;
}

void PID_CTRL::set_resolution(float reso)
{
    resolution = reso;
}

void PID_CTRL::set_output_range(float lower, float upper)
{
    out_range[0] = lower;
    out_range[1] = upper;
}

void PID_CTRL::post_process()
{
    if(output < out_range[0]) output = out_range[0];
    if(output > out_range[1]) output = out_range[1];
}

void PID_CTRL::activate()
{
    state = true;
    i_error = 0;
    prev_error = 0;
}

void PID_CTRL::pause()
{
    state = false;
}

void PID_CTRL::update()
{
    if(!state) return;
    float error = *sp - *input;
    i_error += error;

    if (abs(error) < resolution){
        error = 0;
        i_error = 0;
    }
    float p_term = k[0] * error;
    float i_term = k[0] * k[1] * i_error;
    float d_term = k[0] * k[1] * (error - prev_error);
    output = p_term + i_term + d_term;
    if(p_output != NULL) *p_output = output;
    post_process();
    prev_error = error;
}

void PID_CTRL::set_coef(const float n_k[3])
{
    k = n_k;
}

float* PID_CTRL::get_p_output()
{
    return &output;
}

float* PID_CTRL::get_ext_p_output()
{
    return p_output;
}

float PID_CTRL::get_output()
{
    return output;
}

float PID_CTRL::get_kP()
{
    return k[0];
}

float PID_CTRL::get_kI()
{
    return k[1];
}

float PID_CTRL::get_kD()
{
    return k[2];
}

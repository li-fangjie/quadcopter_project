#include "pid.h"
#include "Arduino.h"


void PID_CTRL::setup(float* n_input, float* n_sp, float n_kP, float n_kI, float n_kD, float n_KF)
{
    input = n_input;
    sp = n_sp;
    kP = n_kP;
    kI = n_kI;
    kD = n_kD;
    kF = n_KF;
    output = NULL;
}

void PID_CTRL::setup(float* n_input, float* n_sp, float k[4])
{
  input = n_input;
  sp = n_sp;
  kP = k[0];
  kI = k[1];
  kD = k[2];
  kF = k[3];
  output = NULL;
}

void PID_CTRL::setup(float* n_input, float* n_output, float* n_sp, float k[4])
{
  input = n_input;
  sp = n_sp;
  kP = k[0];
  kI = k[1];
  kD = k[2];
  kF = k[3];
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
    float p_term = kP * error;
    float i_term = kI * kP * i_error;
    float d_term = kD * kP * (error - prev_error);
    float f_term = 0;
    if(has_up_stream) f_term = kF * (*sp);
    output = p_term + i_term + d_term + f_term;
    if(p_output != NULL) *p_output = output;
    post_process();
    prev_error = error;
}

void PID_CTRL::set_coef(float k_p, float k_i, float k_d, float k_f)
{
    kP = k_p;
    kI = k_i;
    kD = k_d;
    kF = k_f;
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
    return kP;
}

float PID_CTRL::get_kI()
{
    return kI;
}

float PID_CTRL::get_kD()
{
    return kD;
}

float PID_CTRL::get_kF()
{
    return kF;
}

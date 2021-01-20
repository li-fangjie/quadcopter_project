#ifndef __MOTOR_T_H__
#define __MOTOR_T_H__
#include "Arduino.h"
template<typename T_I>
class Motor 
{
  private:
    uint8_t output_pin;
    T_I * p_input;
    // T_I input;

  public:
    void setup(int pin);

    void setup(int pin, T_I * n_p_input);
    
    void operate(T_I val);

    void operate();
    
    // T_I* get_p_input();

    T_I* get_ext_p_input();
};

// #include "motor_t.cpp"
#endif

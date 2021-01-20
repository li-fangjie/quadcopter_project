#include "Motor_t.h"
#include "Arduino.h"

template<typename T_I>
void Motor<T_I>::setup(int pin)
{
	this->output_pin = pin;
  analogWrite(this->output_pin, 0);
  p_input = NULL;
  // input = 0;
}

template<typename T_I>
void Motor<T_I>::setup(int pin, T_I* n_p_input){
  this -> setup(pin);
  p_input = n_p_input;
}

template<typename T_I>
void Motor<T_I>::operate()
{
    if(p_input != NULL) this -> operate(*p_input);
    else operate(0); // Shouldn't happen, but just in case operate() called w/o input pointer set.
    // else operate(input);
}

template<typename T_I>
void Motor<T_I>::operate(T_I val)
{   
    analogWrite(this->output_pin, abs((int)val));
}

template<typename T_I>
T_I* Motor<T_I>::get_ext_p_input()
{
  return p_input;
}

/*
template<typename T_I>
T_I* Motor<T_I>::get_p_input()
{
  return &input;
}
*/

template class Motor<int>;

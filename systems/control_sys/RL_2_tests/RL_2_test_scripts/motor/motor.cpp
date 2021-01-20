#include "Motor.h"
#include "Arduino.h"

void Motor::setup(int pin)
{
	output_pin = pin;
	// pinMode(output_pin, OUTPUT);
  analogWrite(output_pin, 0);
  p_input = NULL;
  input = 0;
}

void Motor::setup(int pin, float* n_p_input){
  this -> setup(pin);
  p_input = n_p_input;
}

void Motor::operate()
{
    if(p_input != NULL) this -> operate(*p_input);
    else operate(input); // Shouldn't happen, but just in case operate() called w/o input pointer set.
}

void Motor::operate(float val)
{   
    analogWrite(output_pin, abs((int)val));
}

float* Motor::get_ext_p_input()
{
  return p_input;
}

float* Motor::get_p_input()
{
  return &input;
}

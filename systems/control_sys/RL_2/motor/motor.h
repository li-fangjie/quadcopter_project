#ifndef __MOTOR_H__
#define __MOTOR_H__


class Motor 
{
  private:
    int output_pin;
    float * p_input;
    float input;

  public:
    motor(){};
    void setup(int pin);

    void setup(int pin, float * n_p_input);
    
    void operate(float val);

    void operate();
    
    float* get_p_input();

    float* get_ext_p_input();
};


#endif

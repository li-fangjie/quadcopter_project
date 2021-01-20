/*
 * Script to test the analog output of motor functions, 
 * using a transistor, pull-down resistors and 2 pins.
 * Result: Both transistors and library methos are functioning 
 * properly.
 */

#define UBRRH
#include "Arduino.h"
#include "motor_t.h"
typedef Motor<int> m;
int value = 150;
m mot;
m mot_2;
m mot_3;
m mot_4;

long t_count = 0;
long p_count = 0;
int out_p = 5;
int in_p = 6;
void setup()
{
    Serial.begin(115200);
    pinMode(out_p, OUTPUT);
    // pinMode(in_p, INPUT);
    mot.setup(5, &value);
    /*
    mot_2.setup(pin);
    mot_3.setup(pin);
    mot_4.setup(pin);
    */
}

void loop()
{
    if ((millis() % 2000) == 0){
        value = millis() % 255;
        mot.operate();
        Serial.println(float(p_count)/t_count);
        t_count = 0;
        p_count = 0;
    } 
    /*else if (millis() % 1000 == 0){
        mot.operate(0);
        Serial.println(float(p_count)/t_count);
        t_count = 0;
        p_count = 0;
    }
    */
    /*
    if (digitalRead(in_p)){
      t_count++;
      p_count++;
    } else{
      t_count++;
    }
    */
}

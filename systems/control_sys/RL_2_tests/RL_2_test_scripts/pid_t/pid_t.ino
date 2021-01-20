#define UBRRH
#include "Arduino.h"
#include "pid_T.h"
typedef PID_CTRL<float, int16_t, float> PID;
typedef PID_CTRL<int16_t, int16_t, float> PID_i;
int pin = 10;
PID pid_1;
PID_i pid_2;

float param[3] = {5, 0.05, 0};

float sensor = 0;
int16_t sensor_i = 0;
float sp = 100;
int16_t output = 0;

long prev_t = 0;


void setup()
{
  Serial.begin(115200);
  pid_2.setup(&sensor_i, &output, &sp, param);
  pid_2.activate();
  // pid_2.setup(&sensor_i, &output, &sp, param);
}

void loop()
{
  if ((millis() - prev_t) > 200) {
    prev_t = millis();
    pid_2.update();
    sensor_i += output * 0.1;
    Serial.println(sensor_i);
  }
    
  
}

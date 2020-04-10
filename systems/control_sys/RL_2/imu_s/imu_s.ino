#define UBRRH
#include "Arduino.h"
#include "imu_s.h"

// int offset[6] = { 1142,     603,    1122,       7,     162,     -35};
float * ypr = NULL;
IMU n_imu;
void dmp_ready()
{
    n_imu.dmp_ready();
}

void setup()
{
    Serial.begin(115200);
    if(n_imu.setup(2, dmp_ready))
        Serial.println("MPU6050 setup failed!");
    else{
        Serial.println("MPU6050 and dmp connected!");
    }
    ypr = n_imu.get_ypr_deg();
}
 
long timer = 0;
void loop()
{
    if(n_imu.get_dmp_ready()){
        int r = n_imu.read_raw();
        if(r!=0){
            Serial.println("Erro!");
        }
        n_imu.update_val();
        if(millis()-timer >= 1){
            timer = millis();
            Serial.print("The ypr are: ");
            Serial.print("Yaw: ");
            Serial.print(ypr[0]);
            Serial.print(" Pitch: ");
            Serial.print(ypr[1]);
            Serial.print(" Roll: ");
            Serial.println(ypr[2]);
        }
    }
}

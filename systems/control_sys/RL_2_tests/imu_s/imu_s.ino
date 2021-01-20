#define UBRRH
#include "Arduino.h"
#include "imu_s.h"

// int offset[6] = { 1142,     603,    1122,       7,     162,     -35};
float * Dypr = NULL;
float * Vypr = NULL;
IMU n_imu;
void dmp_ready()
{
    n_imu.dmp_ready();
}

void setup()
{
    Serial.begin(115200);
    if(n_imu.setup(2, dmp_ready))
        Serial.println(F("MPU6050 setup failed!"));
    else{
        Serial.println(F("MPU6050 and dmp connected!"));
    }
    Dypr = n_imu.get_Dypr();
    Vypr = n_imu.get_Vypr();
}
 
long timer = 0;
void loop()
{
    if(n_imu.get_dmp_ready()){
        int r = n_imu.read_raw();
        if(r!=0){
            Serial.println(F("Error!"));
        }
        n_imu.calc_ypr();
        if(millis()-timer >= 10){
            // timer = millis();
            Serial.print(F("The ypr are: "));
            Serial.print(F("Yaw "));
            Serial.print(Dypr[0]);
            Serial.print(F(" Pit "));
            Serial.print(Dypr[1]);
            Serial.print(F(" Rol "));
            Serial.println(Dypr[2]);
            Serial.print(F("Yaw V "));
            Serial.print(Vypr[0]);
            Serial.print(F(" Pit V "));
            Serial.print(Vypr[1]);
            Serial.print(F(" Rol V "));
            Serial.println(Vypr[2]);
        }
    }
}

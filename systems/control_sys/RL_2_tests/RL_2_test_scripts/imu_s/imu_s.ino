#define UBRRH
#include "Arduino.h"
#include "imu_s.h"

// int offset[6] = { 1142,     603,    1122,       7,     162,     -35};
float * fl_dat_D, * fl_dat_V;
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
    fl_dat_D = n_imu.get_Vz_Dypr();
    fl_dat_V = n_imu.get_Vzypr();
}
 
long timer = 0;
void loop()
{
    if(n_imu.get_dmp_ready()){
        int r = n_imu.read_raw();
        if(r!=0){
            Serial.println(F("Error!"));
        }
        n_imu.update_val();
        if(millis()-timer >= 1){
            timer = millis();
            Serial.print(F("The ypr Discplacements are: "));
            Serial.print(F("Yaw: "));
            Serial.print(fl_dat_D[1]);
            Serial.print(F(" || Pitch: "));
            Serial.print(fl_dat_D[2]);
            Serial.print(F(" || Roll: "));
            Serial.print(fl_dat_D[3]);
            Serial.print(F("Z acc is:"));
            Serial.println(fl_dat_D[0]);
            Serial.print(F("The rates are: "));
            Serial.print(F("Yaw: "));
            Serial.print(fl_dat_V[1]);
            Serial.print(F(" || Pitch: "));
            Serial.print(fl_dat_V[2]);
            Serial.print(F(" || Roll: "));
            Serial.print(fl_dat_V[3]);
            Serial.print(F("Z acc is:"));
            
        }
    }
}

// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#define UBRRH
#include "Arduino.h"
#include<Wire.h>

#define COMP_COEF 0.98
#define CALIB_CT 1000
#define DT 10
#define DT_S 0.01
#define REG_MAX 32767 // 2^15-1
#define BUF_TO_ANG 0.007629627369

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t buf[6] = {0};
float offset[8] = {0};
float ypr[3] = {0};
float Vypr[3] = {0};
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(115200);
  calibrate(offset);
  delay(500);
}
void loop(){
  read_raw(buf);
  calc_pr(buf, offset, ypr, Vypr);


  /*
  Serial.print("AcX = "); Serial.print(buf[0]);
  Serial.print(" | AcY = "); Serial.print(buf[1]);
  Serial.print(" | AcZ = "); Serial.print(buf[2]);
  // Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(buf[3] - offset[3]);
  Serial.print(" | GyY = "); Serial.print(buf[4] - offset[4]);
  Serial.print(" | GyZ = "); Serial.println(buf[5] - offset[5]);
  */
  
  Serial.print("Pitch = "); 
  Serial.print(ypr[1]);
  Serial.print(" | Roll = "); Serial.println(ypr[2]);
  Serial.print("Pitch V = "); 
  Serial.print(Vypr[1]);
  Serial.print(" | Roll V = "); 
  Serial.println(Vypr[2]);

  delay(DT);
}


void read_raw(int16_t * buf) 
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    buf[0] = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    buf[1] = Wire.read()<<8|Wire.read();  
    buf[2] = Wire.read()<<8|Wire.read();  
    int16_t temp = Wire.read()<<8|Wire.read();
    buf[5] = Wire.read()<<8|Wire.read();
    buf[4] = Wire.read()<<8|Wire.read();
    buf[3] = Wire.read()<<8|Wire.read();
    Serial.println(buf[5]);
    
      /*
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      */
}

void calibrate(float * offset)
{
    delay(1500);
    Serial.println(F("Calibrating IMU..."));
    Serial.print(F("Loop Count: "));
    Serial.println(CALIB_CT);
    int16_t temp[6];
    for(int i=0; i<CALIB_CT; ++i) {
        read_raw(temp);
        for(int j=0; j<6; ++j) {
            offset[j] += temp[j];
        }
        delay(5);
        if(i % (CALIB_CT/10) == 0) {
            //Serial.print(F("."));
        }
    }
    for (int i=0; i<6; ++i) {
        offset[i] /= CALIB_CT;
        // Serial.println(offset[i]);
    }
    Serial.println(offset[5]);
    offset[6] = atan2(offset[0], offset[2]);
    offset[7] = atan2(offset[1], offset[2]);
    // Serial.println(offset[6]);
    // Serial.println(offset[7]);
    delay(100);
    Serial.println();
    Serial.println(F("Calibration Completed"));
}

void calc_pr(int16_t * buf, float * offset, float * ypr, float * V_ypr)
{
    // for pitch(around y), the angle from acc should be x, z -> [0] / [1]
    for (int i=1; i<3; ++i) {
        // float base = sqrt((float)buf[2] * buf[2] + buf[2-i] * buf[2-i]);
        float prev_D = ypr[i];
        ypr[i] += (buf[i+3] - offset[i+3]) * BUF_TO_ANG * DT_S;
        // if (abs(buf[i-1]) > 328) {
        float acc_th = (atan2(buf[i-1], buf[2]) - offset[i+5]) * RAD_TO_DEG;
        if(i==1) acc_th = -acc_th;
        ypr[i] = ypr[i] * COMP_COEF + acc_th * (1-COMP_COEF);
        // }
        V_ypr[i] += ((ypr[i] - prev_D) / DT_S - V_ypr[i])/3;
    }
}

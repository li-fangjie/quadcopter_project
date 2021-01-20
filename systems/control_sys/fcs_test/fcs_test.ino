#define UBRRH
#include "Arduino.h"
#include "imu_t.h"
#include "rx.h"

// int offset[6] = { 1142,     603,    1122,       7,     162,     -35};
float vy_pid_coef[3] = {1, 0.3, 0.1};

float dp_pid_coef[3] = {2, 0.4, 0.1};
float vp_pid_coef[3] = {0.5, 0.3, 0.1};

float dr_pid_coef[3] = {2, 0.4, 0.1};
float vr_pid_coef[3] = {0.5, 0.3, 0.1};


float * Vypr = NULL;
float * Dypr = NULL;
float flight_data[6];

RF24 radio(7,8);
Rx rx(&radio);
IMU imu;
uint8_t * ctrl = NULL;
// float v_ypr[4] = {0, 0, 0, 0};
/*
uint8_t * ctrl = NULL;
FCS main_fcs;
PID_YPR_CTRL pids[4] = {PID_YPR_CTRL(), PID_YPR_CTRL(), PID_YPR_CTRL(), PID_YPR_CTRL()};
Motor_i mots[4] = {Motor_i(), Motor_i(), Motor_i(), Motor_i()};
RF24 radio(7,8);
Rx rx(&radio)
*/

void setup()
{
    uint8_t setup_success = 1;
    Serial.begin(115200);
    Serial.println();
    Wire.begin();
    
    if(rx.setup()) {
        Serial.println(F("Radio Setup failed!"));
        setup_success = 0;
    }

    /*
    rx.setup();
    mots[0].setup(9);
    mots[1].setup(9);
    mots[2].setup(9);
    mots[3].setup(9);
    if(n_imu.setup(2, dmp_ready))
        Serial.println("MPU6050 setup failed!");
    else{
        Serial.println("MPU6050 and dmp connected!");
    }

    main_fcs.attach_mot(mots);
    main_fcs.attach_pid(pids, pids+1, pids+2, pids+3);
    main_fcs.attach_rx(&rx);
    main_fcs.setup_pid(y_pid_coef, p_pid_coef, r_pid_coef, a_pid_coef);

    main_fcs.activate_pid();
    main_fcs.config_pid();
    ctrl = rx.get_buf();
    */
}

void loop()
{
  
}


unsigned long timer = 0;
unsigned long timer_1 = 0;

void loop_0()
{
    memcpy(flight_data, Dypr, sizeof(float)*3);
    memcpy(&flight_data[3], Vypr, sizeof(float)*3);
    if(!rx.update_ctrl(flight_data)) {
        // Serial.println("received!");
    }
    if(millis() - timer >= UPDATE_T_MS) {
        timer = millis();
        // Serial.println(millis()-timer_1);
        timer_1 = millis();
        imu.read_raw();
        imu.calc_ypr();
        analogWrite(6, millis()%255);
        Serial.println("Cur Data:");
        Serial.print(F("Yaw ")); Serial.print(Dypr[0]);
        Serial.print(F(" Pit ")); Serial.print(Dypr[1]);
        Serial.print(F(" Rol ")); Serial.println(Dypr[2]);
        
        Serial.print(F("Yaw V ")); Serial.print(Vypr[0]);
        Serial.print(F(" Pit V ")); Serial.print(Vypr[1]);
        Serial.print(F(" Rol V ")); Serial.println(Vypr[2]);
        
        /*
        Serial.print("Rcvd Ctrl:");
        Serial.print(ctrl[0]); Serial.print(" | "); 
        Serial.print(ctrl[1]); Serial.print(" | "); 
        Serial.print(ctrl[2]); Serial.print(" | "); 
        Serial.println(ctrl[3]); // Serial.print(" | "); 
        */
    }
}

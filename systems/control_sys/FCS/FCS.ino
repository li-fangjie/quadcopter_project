#define UBRRH
#include "Arduino.h"
/*
#include "imu_s.h"
#include "rx.h"
#include "pid_t.h"
#include "motor_t.h"
*/
#include "fcs.h"

/****** PID COEFFICIENTS *****/
// {PIT, ROL}
// {10.5, 0.5, 8}
float d_pid_coefs[D_PID_CT][3] = {{10.5, 0.55, 8}, {10.5, 0.55, 8}}; // 11 0.6 10
// {YAW, PIT, ROL}
// MODE 1/2:
// Baseline: 0.11, 0.00009, 0.33
float v_pid_coefs[V_PID_CT][3] = {{0.6, 0.00005, 0.1}, {0.11, 0.00009, 0.3}, {0.11, 0.00009, 0.3}}; // I = 0.00008?
// MODE 0:
// float v_pid_coefs[V_PID_CT][3] = {{1, 0.3, 0.1}, {0.4, 0, 1}, {0.25, 0.000, 1}};

/****** MOTOR PINS ******/
uint8_t mot_pins[4] = {5, 6, 10, 9};

/****** data *****/
float * Vypr = NULL;
float * Dypr = NULL;
uint8_t * ctrl = NULL;
int16_t * pid_o = NULL;
int16_t * raw = NULL;
uint8_t * mot_o  = NULL;
// float flight_data[6];

/****** OBJECTS ******/
FCS fcs;

RF24 radio(7,8); // ce, cs
Rx rx(&radio);
IMU imu;
// {YAW_V_CTRL, PIT_V_CTRL, ROL_PITCH_CTRL}
PID_V_CTRL v_pids[3] = {PID_V_CTRL(), PID_V_CTRL(), PID_V_CTRL()};
// {PIT_V_CTRL, ROL_V_CTRL}
PID_D_CTRL d_pids[2] = {PID_D_CTRL(), PID_D_CTRL()};
Motor_i mots[4] = {Motor_i(), Motor_i(), Motor_i(), Motor_i()};

uint8_t ctrl_mode = 1;
/****** DEBUG & TUNING ******/
// float v_ypr[4] = {0, 0, 0, 0};
/*
uint8_t * ctrl = NULL;
FCS main_fcs;
PID_YPR_CTRL pids[4] = {PID_YPR_CTRL(), PID_YPR_CTRL(), PID_YPR_CTRL(), PID_YPR_CTRL()};
Motor_i mots[4] = {Motor_i(), Motor_i(), Motor_i(), Motor_i()};
RF24 radio(7,8);
Rx rx(&radio)
*/

#ifdef USE_IMU_S
void check_dmp_ready()
{
    imu.dmp_ready();
}
#endif

void set_pid(float pid_coef[3])
{
    bool p_p_set = false;
    do {
        if(Serial.available()) {
            for(int i=0; i<3; i++) {
                float coef = Serial.parseFloat();
                if(coef < 0) {
                  break;
                }
                pid_coef[i] = coef;
            }
            p_p_set = true;
        } 
    } while(p_p_set == false);
    for(int i=0; i<3; ++i) {
        Serial.print(pid_coef[i]);
        Serial.print(F(", "));
    }
    Serial.println();
}

int set_mode(bool * v_pid_set, bool * d_pid_set)
{
    bool mode_set = false;
    Serial.println("Please select op mode(0-2): ");
    while(mode_set == false) {
        if(Serial.available()) {
            char mode = Serial.read();
            switch (mode) {
                case '2':
                    *v_pid_set = true;
                    *d_pid_set = false;
                    return 2;
                    break;
                case '0':
                    *v_pid_set = true;
                    *d_pid_set = false;
                    mode_set = true;
                    return 0;
                    break;
                case '1':
                    *d_pid_set = true;
                    Serial.println("Should default v_pid be used? y/n");
                    while(!Serial.available());
                    char v_mode = Serial.read();
                    switch (v_mode) {
                        case 'y':
                            *v_pid_set = false;
                            break;
                        default:
                            *v_pid_set = true;
                            break;
                    }
                    return 1;
                    mode_set = true;
                    break;
            }
        }
    }

}
bool set_v_pid = true;
bool set_d_pid = true;

void setup()
{
    uint8_t setup_success = 1;
    Serial.begin(115200);
    Wire.begin();

    fcs.attach_imu(&imu);
    fcs.attach_pid(d_pids, v_pids);
    fcs.attach_rx(&rx);
    fcs.attach_mot(mots);
    fcs.attach_bat_val(BAT_VAL_PIN, BAT_LED_PIN);
    
    for(int i=0; i<4; ++i) {
        mots[i].setup(mot_pins[i]);
    }

    Serial.println(F("***Setting up Rx radio...***"));
    if(rx.setup()) {
        Serial.println(F("Radio setup failed!"));
        setup_success = 0;
        while(true);
    } else {
        Serial.println(F("***Rx radio setup completed***"));
    }

    /****** DEBUG/TUNING PURPOSE ******/
    ctrl_mode = set_mode(&set_v_pid, &set_d_pid);
    Serial.print(F("The mode is: ")); Serial.println(ctrl_mode);
    if(set_v_pid) {
        Serial.println(F("please set the roll V_pid coefficients"));
        set_pid(v_pid_coefs[2]);
        Serial.println(F("please set the pitch V_pid coefficients"));
        set_pid(v_pid_coefs[1]);
    }
    if (set_d_pid) {
        Serial.println(F("please set the roll D_pid coefficients"));
        set_pid(d_pid_coefs[1]);
        Serial.println(F("please set the pitch D_pid coefficients"));
        set_pid(d_pid_coefs[0]);
    }
    Serial.println(F("please set the yaw V_pid coefficients"));
    set_pid(v_pid_coefs[0]);
    Serial.println(F("Debug config completed."));

    // Setup the pid after the coefficients are set
    fcs.setup_pid(d_pid_coefs, v_pid_coefs);
    
    fcs.set_state(1); // arm and start operation
    // fcs.arm(); // pair up the controller and drone.
    Serial.println(F("***Setting up IMU unit...***"));
    #ifdef USE_IMU_S
        if(imu.setup(2, check_dmp_ready)) {
    #endif
    #ifdef USE_IMU_T
        if(imu.setup(false)) {
    #endif
        Serial.println(F("IMU setup failed!"));
        setup_success = 0;
    } else {
        Serial.println(F("***IMU Setup completed***"));
    }
    
    if(!setup_success) {
        Serial.println(F("Setup sequence failed, please check error message above and attempt restart"));
        while(!setup_success);
    }    

    Vypr = imu.get_Vypr();
    Dypr = imu.get_Dypr();
    ctrl = rx.get_buf_body();
    pid_o = fcs.get_inner_pid_o();
    // raw = imu.get_raw();
    mot_o = fcs.get_mot_o();
    /*
    for(int i=0;i<1000;i++) {
        imu.read_raw();
        Serial.println(raw[5]);
        delay(5);
    }
    Serial.println("end");
    */
    // fcs.activate_pid();
    fcs.set_ctrl_mode(ctrl_mode); // properly set control_mode
    Serial.print(F("CONFIG REG: ")); Serial.println(imu.read_reg(CONFIG));
}
 
unsigned long timer = 0;
unsigned long timer_1 = 0;
bool calibrated = false;
int i = 0;
uint8_t outer_counter;

void loop()
{
    
    if(!calibrated) {
      imu.calibrate();
      calibrated = true;
    }
    /*
    if(imu.get_dmp_ready()){
       // fcs.update_imu(); 
       // imu.read_raw();
    }
    */

    if(fcs.rcv_ctrl() == 0) {
       fcs.map_ctrl();
       /*
        Serial.print("Rcvd Ctrl:");
        Serial.print((uint8_t)ctrl[0]); Serial.print(" | "); 
        Serial.print((int8_t)ctrl[1]); Serial.print(" | "); 
        Serial.print((int8_t)ctrl[2]); Serial.print(" | "); 
        Serial.println((int8_t)ctrl[3]); // Serial.print(" | "); 
        */
    }
    if(outer_counter > OUTER_P) {
      fcs.update_outer_pid();
      outer_counter = 0;
    }
    if(millis() - timer_1 >= MISC_UT_MS) {
        timer_1 = millis();
        fcs.update_bat_val();
        
        Serial.println(fcs.get_bat_val());
        // Serial.print("the current state:"); Serial.println(fcs.get_cur_state());
        
        /*
        Serial.print(F("Y ")); Serial.print(Dypr[0]);
        Serial.print(F(" P ")); Serial.print(Dypr[1]);
        Serial.print(F(" R ")); Serial.println(Dypr[2]);
        
        
        Serial.print(F("YV ")); Serial.print(Vypr[0]);
        Serial.print(F(" PV ")); Serial.print(Vypr[1]);
        Serial.print(F(" RV ")); Serial.println(Vypr[2]);
        */
        /*
        Serial.print(F("M0 ")); Serial.print(mot_o[0]);
        Serial.print(F(" M1 ")); Serial.print(mot_o[1]);
        Serial.print(F(" M2 ")); Serial.print(mot_o[2]);
        Serial.print(F(" M3 ")); Serial.println(mot_o[3]);
        */
        /*
        memcpy(temp, &(imu -> get_Dypr())[1], sizeof(float));
        memcpy(&temp[1], &(imu -> get_Vypr())[1], sizeof(float));
        temp[2] = ((float)inner_pid_o[2]);
        temp[3] = ((float)pypr_sp[2]);
        temp[4] = v_pids[1].get_error(); 
        temp[5] = v_pids[1].get_i_error();
         */
        /*
        // Serial.print(Dypr[0]); Serial.print(F(", ")); 
        Serial.print(Dypr[1]); Serial.print(F(", ")); 
        Serial.print(Dypr[2]); Serial.println(F(", ")); 
        */
        /*
        // Serial.print(Dypr[2]); Serial.print(F(", ")); 
        // Serial.print(Vypr[0]); Serial.print(F(", ")); 
        Serial.print(Vypr[1]); Serial.print(F(", ")); 
        // Serial.print(Vypr[2]); Serial.print(F(", ")); 
        Serial.print(v_pids[1].get_error()); Serial.print(F(", "));
        Serial.print(v_pids[1].get_i_error()); Serial.print(F(", "));
        Serial.println(pid_o[2]);
        */
        
        /*
        // Serial.println(raw[4] * BUF_TO_ANG); // Serial.print(F(", ")); 
        /*
        Serial.print(F("PID: "));
        Serial.print(pid_o[1]); Serial.print(", ");
        Serial.print(pid_o[2]); Serial.print(", ");
        Serial.println(pid_o[3]); // Serial.println(", ");
        */
        // imu.read_raw();
        // Serial.print("RAW ");
        // Serial.println(raw[5]);
        /*
        for (int i=0; i<6; ++i) {
            Serial.print(raw[i]);
            Serial.print(" | ");
        }
        Serial.println( );
        */
    }
    if(millis() - timer >= UPDATE_T_MS) {
        // Serial.println(millis()-timer);
        timer = millis();
        fcs.update_imu();
        // imu.calc_ypr();
        fcs.update_inner_pid();
        fcs.map_motors_o();
        fcs.op_motors();
        // imu.read_raw();
        // mots[0].operate(5);
        // imu.calibrate();
        // Serial.println(millis() - timer_1);
        ++outer_counter;
    }
}

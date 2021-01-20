#include "fcs.h"

FCS::FCS()
: imu(NULL), rx(NULL), mots(NULL), pypr_sp{}, outer_pid_o{}, inner_pid_o{}, mot_o{}, fl_dat(NULL), ctrl_mode(1),  cur_state(0)
{}

void FCS::attach_mot(Motor_i * m_s)
{
    mots = m_s;
}

void FCS::attach_rx(Rx * n_rcv)
{ 
    rx = n_rcv;
}

void FCS::attach_imu(IMU * n_imu)
{
    imu = n_imu;
}

void FCS::attach_bat_val(uint8_t bv_pin, uint8_t bl_pin)
{
    bat_val_pin = bv_pin;
    bat_led_pin = bl_pin;
    pinMode(bat_led_pin, OUTPUT);
}

void FCS::attach_pid(PID_D_CTRL * outer_pids, PID_V_CTRL * inner_pids)
{
    d_pids = outer_pids;
    v_pids = inner_pids;
}

void FCS::setup_pid(float outer_coefs[][3], float inner_coefs[][3])
{
    // Template based pid_t; setting up each pid, w/ coefficients supplied.
    // Displacement pids:
    for(int i=0; i<D_PID_CT; ++i) {
        d_pids[i].setup(&(imu->get_Dypr()[i+1]), &outer_pid_o[i], &pypr_sp[i+2], outer_coefs[i]);
    }
    v_pids[0].setup(&(imu->get_Vypr()[0]), &inner_pid_o[1], &pypr_sp[1], inner_coefs[0]);
    for (int i=1; i<V_PID_CT; ++i) {
        v_pids[i].setup(&(imu->get_Vypr()[i]), &inner_pid_o[i+1], &outer_pid_o[i-1], inner_coefs[i]);
    }

    config_pid();
}

void FCS::config_pid()
{
    v_pids[0].set_output_range(-YAW_O_MAG, YAW_O_MAG);
    v_pids[1].set_output_range(-PITCH_O_MAG, PITCH_O_MAG);
    v_pids[2].set_output_range(-ROLL_O_MAG, ROLL_O_MAG);

    for(int i=0; i<V_PID_CT; ++i){
        v_pids[i].set_resolution(V_RESO);
    }
    for(int i=0; i<D_PID_CT; ++i){
        d_pids[i].set_resolution(D_RESO);
    }
}

void FCS::activate_pid()
{
    for(int i=0; i<D_PID_CT; ++i) {
        d_pids[i].activate();
    }
    for (int i=0; i<V_PID_CT; ++i) {
        v_pids[i].activate();
    }
}

void FCS::pause_pid()
{
    for(int i=0; i<D_PID_CT; ++i) {
        d_pids[i].pause();
    }
    for (int i=0; i<V_PID_CT; ++i) {
        v_pids[i].pause();
    }   
}

void FCS::arm()
{
    uint8_t cur_pwr = 0;
    float temp_ack[ACK_LEN] = {0};
    temp_ack[2] = cur_state;
    while(cur_pwr < PWR_MAX - 10) {
        rx->update_ctrl(temp_ack);
        cur_pwr = *rx->get_buf_body();
    }
    while(cur_pwr > PWR_MIN + 10) {
        rx->update_ctrl(temp_ack);
        cur_pwr = *rx->get_buf_body();
    } 
    mots[0].operate(5);
    delay(10);
    mots[0].operate(0);
    // activate_pid();
}

void FCS::set_state(uint8_t target)
{
    if(target == cur_state || target > 3) return;
    if(target == 1) {
        // Serial.println("du");
        if(cur_state != 3) {
            arm();
        }
        activate_pid();
        set_ctrl_mode(ctrl_mode); // Activate the pids based on ctrl mode
    } else if(target == 2 || target == 3) {
        uint8_t * ctrl_buf = rx -> get_buf_body();
        memset(mot_o, 0, 4 * sizeof(uint8_t));
        memset(inner_pid_o, 0, 4 * sizeof(uint16_t));
        memset(outer_pid_o, 0, 2 * sizeof(float));
        memset(pypr_sp, 0, 4 * sizeof(float));
        memset(ctrl_buf, 0, 4 * sizeof(uint8_t));
        for(int i=0; i<4; ++i) {
            mots[i].operate(0);
        }
        pause_pid();
    }
    cur_state = target;
}

int FCS::rcv_ctrl()
{
    /*** Create an array of ack data(telemetry) to return ***/
    float temp[ACK_LEN];
    int16_t * raw_data = imu -> get_raw();
    if(ctrl_mode==1 || ctrl_mode == 0) {
        memcpy(temp, &(imu -> get_Dypr())[1], sizeof(float)*2);
    } else if(ctrl_mode==2) {
        memcpy(temp, &(imu -> get_Vypr())[1], sizeof(float)*2);
    }
    // temp[2] = ((float)inner_pid_o[3]);
    // temp[3] = ((float)pypr_sp[3]);
    temp[2] = (float)inner_pid_o[1];
    temp[3] = ((float)inner_pid_o[2]);
    temp[4] = ((float)inner_pid_o[3]);
    temp[5] = (float)cur_state;
    // temp[3] = (float)mot_o[2];
    
    // memcpy(temp+2, &(imu -> get_Dypr())[1], sizeof(float)*2);
    // temp[5] = v_pids[2].get_error();
    // temp[5] = v_pids[1].get_i_error();
    // temp[4] = (raw_data[4] - imu -> get_offset_raw()[4]) * BUF_TO_ANG;
    // temp[5] = atan2(raw_data[0], raw_data[2] - imu -> get_acc_offset_ang()[0]) * RAD_TO_DEG;
    /*** Actually try to receive data ***/
    int result = rx->update_ctrl(temp);
    /*** Depending on data availability, set states, etc ***/
    if(!result) { // data received
        if(cur_state == 2) {
            set_state(1); // if conn resumed, reset state to 1
        }
        return 0;
    } else if(millis() - rx->get_last_ctrl_t() > MAX_DELAY_T) {
        set_state(2); // if no conn in a while, assume loss of conn.
        return 2; // timeout error;
    }
    return 1;
}

int FCS::update_imu()
{
    imu -> read_raw();
    imu -> calc_ypr();
    return 0;
}

void FCS::map_ctrl()
{
    uint8_t * ctrl_buf = rx -> get_buf_body();
    /* 
    pypr_sp[0] = (ctrl_buf[0] - 128) * 2;
    pypr_sp[1] = (ctrl_buf[1] - 128) * VYPR_SP_COEF; 
    if(ctrl_mode == 1) {
        pypr_sp[2] = (ctrl_buf[2] - 128) * DYPR_SP_COEF;
        pypr_sp[3] = (ctrl_buf[3] - 128) * DYPR_SP_COEF;
    } else if(ctrl_mode == 2) {
        pypr_sp[2] = (ctrl_buf[2] - 128) * VYPR_SP_COEF;
        pypr_sp[3] = (ctrl_buf[3] - 128) * VYPR_SP_COEF;
    }
    */
   // The commands should be signed char, indicating direction + magnitude.
   // Here, the analog val is converted to setpoint value with units.
   // power is the Exception for now: It is passed directly to output, as uint8_t (0-255)
    pypr_sp[0] = (uint8_t)ctrl_buf[0];
    pypr_sp[1] = ((int8_t)ctrl_buf[1]) * VYPR_SP_COEF; 
    if(ctrl_mode == 1 || ctrl_mode == 0) {
        pypr_sp[2] = (int8_t)ctrl_buf[2] * DYPR_SP_COEF;
        pypr_sp[3] = (int8_t)ctrl_buf[3] * DYPR_SP_COEF;
    } else if(ctrl_mode == 2) {
        pypr_sp[2] = (int8_t)ctrl_buf[2] * VYPR_SP_COEF;
        pypr_sp[3] = (int8_t)ctrl_buf[3] * VYPR_SP_COEF;
    }
    if(pypr_sp[0] == 0) {
        set_state(3);
    } else if(cur_state == 3 && pypr_sp[0] != 0) {
        set_state(1);
    }
}

void FCS::update_pid()
{
    // pow_pid -> update();
    // yaw_pid -> update();
    // outer_pid_o[0] = pypr_sp[0]; // Black magic (No). Direct fed w/o pid.
    // outer_pid_o[1] = pypr_sp[1]; // Direct fed w/o pid.
    if(cur_state != 1) return;
    if(ctrl_mode == 1) {
        for(int i=0; i<D_PID_CT; ++i) {
            d_pids[i].update();
        }
    }
    for (int i=0; i<V_PID_CT; ++i) {
        v_pids[i].update();
    }
    inner_pid_o[0] = pypr_sp[0];
}

void FCS::update_outer_pid()
{   
    if(cur_state != 1) return;
    if(ctrl_mode == 1) {
        for(int i=0; i<D_PID_CT; ++i) {
            d_pids[i].update();
        }
    }
}

void FCS::update_inner_pid()
{
    if(cur_state != 1) return;
    if(pypr_sp[0] == 0) {
        for(int i=0; i<V_PID_CT; ++i) {
            v_pids[i].activate(); // Reset the error
            inner_pid_o[i+1] = 0;
        }
    } else {
        for (int i=0; i<V_PID_CT; ++i) {
            v_pids[i].update();
        }
    }
    inner_pid_o[0] = pypr_sp[0];
}

void FCS::map_motors_o()
{
  // TODO: sort out the screwed order of motors.
          /*
        m_0:5    m_1:6
          \    /
           \__/
           |/\|
           /  \
          /    \
        m_3:9    m_2:10
        */
    if(cur_state != 1) return;
    int temp_o[4];

    temp_o[0] = inner_pid_o[0] + inner_pid_o[1] - inner_pid_o[2] + inner_pid_o[3];
    temp_o[1] = inner_pid_o[0] - inner_pid_o[1] - inner_pid_o[2] - inner_pid_o[3];
    temp_o[2] = inner_pid_o[0] + inner_pid_o[1] + inner_pid_o[2] - inner_pid_o[3];
    temp_o[3] = inner_pid_o[0] - inner_pid_o[1] + inner_pid_o[2] + inner_pid_o[3]; 
    
    /*
    temp_o[0] = inner_pid_o[0] + inner_pid_o[3];
    temp_o[1] = inner_pid_o[0] - inner_pid_o[3];
    temp_o[2] = (inner_pid_o[0] - inner_pid_o[3]) * (int)B_MOT_COEF;
    temp_o[3] = (inner_pid_o[0] + inner_pid_o[3]) * (int)B_MOT_COEF; 
    */
    
    for (int i=0; i<4; i++) {
        if (temp_o[i] < 0) {
            temp_o[i] = 0;
        } else if(temp_o[i] > 255) {
            temp_o[i] = 255;
            
        }
        mot_o[i] = temp_o[i];
    }
}


void FCS::op_motors()
{
    if(cur_state != 1) {
        for(int i=0; i<4; ++i) {
            mots[i].operate(0);
        }
        return;
    }
    mots[0].operate(mot_o[0]);
    mots[1].operate(mot_o[1]);
    mots[2].operate(mot_o[2]);
    mots[3].operate(mot_o[3]);
}

void FCS::update_bat_val()
{
    bat_val = bat_val * 4/5 + analogRead(bat_val_pin)/5;
    if(bat_val < BAT_MIN) {
        digitalWrite(bat_led_pin, HIGH);
    } else {
        digitalWrite(bat_led_pin, LOW);
    }
}

float * FCS::get_pypr_sp()
{
    return pypr_sp;
}

int16_t * FCS::get_inner_pid_o()
{
    return inner_pid_o;
}

uint8_t * FCS::get_mot_o()
{
    return mot_o;
}

uint16_t FCS::get_bat_val()
{
    return bat_val;
}

uint8_t FCS::set_ctrl_mode(uint8_t mode)
{ 
    // if(mode == ctrl_mode) return 0;
    /*
    for (int i=0; i<V_PID_CT; ++i) {
        v_pids[i].set_coef(inner_coefs[i]);
    }
    */
    if(mode == 1) {
        for (int i=0; i<D_PID_CT; ++i) {
            d_pids[i].set_resolution(D_RESO);
            d_pids[i].activate();
        }
        for (int i=1; i<V_PID_CT; ++i) {
            v_pids[i].set_resolution(V_RESO);
            v_pids[i].set_sp(&outer_pid_o[i-1]);
        }
    } else if(mode == 2) {
        for (int i=0; i<D_PID_CT; ++i) {
            d_pids[i].pause();
        }
        for (int i=1; i<V_PID_CT; ++i) {
            v_pids[i].set_resolution(V_RESO);
            v_pids[i].set_sp(&pypr_sp[i+1]);
        }
    } else if(mode == 0) {
        for (int i=0; i<D_PID_CT; ++i) {
            d_pids[i].pause();
        }
        for (int i=1; i<V_PID_CT; ++i) {
            v_pids[i].set_sp(&pypr_sp[i+1]);
            v_pids[i].set_input(&(imu -> get_Dypr()[i]));
            v_pids[i].set_resolution(D_RESO);
        }
    } else {
        return 1;
    }
    ctrl_mode = mode;
    return 0;
}

uint8_t FCS::get_ctrl_mode()
{
    return ctrl_mode;
}

uint8_t FCS::get_cur_state()
{
    return cur_state;
}

/*
void FCS::setup_pid(float * y_coef, float * p_coef, float * r_coef, float * pow_coef)
{
    // Super sketchy way of passing float input/setpoint to pid control... 
    // -> by creating specific interface in respective classes (radio/imu)
    // A better approach will be template.
    pow_pid -> setup(imu -> get_awz(), rcv -> get_f_buf(), pow_coef);
    yaw_pid -> setup(imu -> get_ypr_deg(), rcv -> get_f_buf() + 1, y_coef);
    pit_pid -> setup(imu -> get_ypr_deg() + 1, rcv -> get_f_buf() + 2, p_coef);
    rol_pid -> setup(imu -> get_ypr_deg() + 2, rcv -> get_f_buf() + 3, r_coef);
}
*/

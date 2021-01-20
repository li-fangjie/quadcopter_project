#include "fcs.h"

FCS::FCS()
: imu(NULL), rx(NULL), mots(NULL), pypr_sp{}, outer_pid_o{}, mot_o{}, fl_dat(NULL), ctrl_mode(1)
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

void FCS::attach_pid(PID_D_CTRL * outer_pids, PID_V_CTRL * inner_pids)
{
    d_pids = outer_pids;
    v_pids = inner_pids;
}

void FCS::setup_pid(float * outer_coefs[3], float * inner_coefs[3])
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
}

void FCS::config_pid()
{
    d_pids[0].set_output_range(-YAW_O_MAG, YAW_O_MAG);
    d_pids[1].set_output_range(-PITCH_O_MAG, PITCH_O_MAG);
    d_pids[2].set_output_range(-ROLL_O_MAG, ROLL_O_MAG);
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

int FCS::rcv_ctrl()
{
    // CHECK THIS...
    float temp[ACK_LEN];
    memcpy(temp, imu->get_Dypr(), sizeof(float)*3);
    memcpy(&temp[3], imu->get_Vypr(), sizeof(float)*3);
    return rx->update_ctrl(temp);
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
    pypr_sp[0] = (int16_t)(ctrl_buf[0]);
    pypr_sp[1] = (int16_t)(ctrl_buf[1] - 128) * VYPR_SP_COEF; 
    if(ctrl_mode == 1) {
        pypr_sp[2] = (int16_t)(ctrl_buf[2] - 128) * DYPR_SP_COEF;
        pypr_sp[3] = (int16_t)(ctrl_buf[3] - 128) * DYPR_SP_COEF;
    } else if(ctrl_mode == 2) {
        pypr_sp[2] = (int16_t)(ctrl_buf[2] - 128) * VYPR_SP_COEF;
        pypr_sp[3] = (int16_t)(ctrl_buf[3] - 128) * VYPR_SP_COEF;
    }
}

void FCS::update_pid()
{
    // pow_pid -> update();
    // yaw_pid -> update();
    // outer_pid_o[0] = pypr_sp[0]; // Black magic (No). Direct fed w/o pid.
    // outer_pid_o[1] = pypr_sp[1]; // Direct fed w/o pid.
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

void FCS::map_motors_o()
{
          /*
        m_1:5    m_2:6
          \    /
           \__/
           |/\|
           /  \
          /    \
        m_3:9    m_4:10
        */
    mot_o[0] = inner_pid_o[0] - inner_pid_o[1] - inner_pid_o[2] - inner_pid_o[3];
    mot_o[1] = inner_pid_o[0] + inner_pid_o[1] - inner_pid_o[2] + inner_pid_o[3];
    mot_o[2] = inner_pid_o[0] - inner_pid_o[1] + inner_pid_o[2] - inner_pid_o[3];
    mot_o[3] = inner_pid_o[0] + inner_pid_o[1] + inner_pid_o[2] + inner_pid_o[3]; 
}


void FCS::op_motors()
{
    mots[0].operate(mot_o[0]);
    mots[1].operate(mot_o[1]);
    mots[2].operate(mot_o[2]);
    mots[3].operate(mot_o[3]);
}

float * FCS::get_pypr_sp()
{
    return pypr_sp;
}

uint8_t FCS::set_ctrl_mode(uint8_t mode, float * inner_coefs[3])
{ // TODO: FINISH THIS!
    if(mode == ctrl_mode) return 0;
    ctrl_mode = mode;
    for (int i=0; i<V_PID_CT; ++i) {
        v_pids[i].set_coef(inner_coefs[i]);
    }
    if(ctrl_mode = 1) {
        for (int i=0; i<D_PID_CT; ++i) {
            d_pids[i].activate();
        }
        for (int i=0; 1<V_PID_CT; ++i) {
            v_pids[i].set_sp(&outer_pid_o[i-1]);
        }
    } else if(ctrl_mode = 2) {
        for (int i=0; i<D_PID_CT; ++i) {
            d_pids[i].pause();
        }
        for (int i=1; i<V_PID_CT; ++i) {
            v_pids[i].set_sp(&pypr_sp[i+1]);
        }
    }
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

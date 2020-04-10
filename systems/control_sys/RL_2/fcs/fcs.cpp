#include "fcs.h"
#include "../imu_s/imu_s.h"

FCS::FCS()
: imu(NULL), rcv(NULL), mot({NULL})
{}

void  FCS::attach_mot(Motor * m_a, Motor * m_b, Motor * m_c, Motor * m_d)
{
    mot[0] = m_a;
    mot[1] = m_b;
    mot[2] = m_c;
    mot[3] = m_d;
}

void FCS::attach_mot(Motor * m_s)
{
    memcpy(mot, m_s, 4 * sizeof(Motor *));
}

void FCS::attach_rx(Rx * n_rcv)
{
    rcv = n_rcv;
}

void FCS::attach_imu(IMU * n_imu)
{
    imu = n_imu;
}

void FCS::attach_pid(PID_YPR_CTRL * yaw, 
                    PID_YPR_CTRL * pit, 
                    PID_YPR_CTRL * rol, 
                    PID_ACC_CTRl * pow)
{
    yaw_pid = yaw;
    pit_pid = pit;
    rol_pid = rol;
    pow_pid = pow;
}

void FCS::setup_pid(float * y_coef, float * p_coef, float * r_coef, float * pow_coef)
{
    // Template based pid_t
    pow_pid -> setup(&(imu -> get_p_aa_world() -> z), mot_o, pypr_sp, pow_coef);
    yaw_pid -> setup(imu -> get_ypr_deg(), mot_o + 1, pypr_sp + 1, y_coef);
    pit_pid -> setup(imu -> get_ypr_deg() + 1, mot_o + 2, pypr_sp + 2, p_coef);
    rol_pid -> setup(imu -> get_ypr_deg() + 2, mot_o + 3, pypr_sp + 3, r_coef);
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
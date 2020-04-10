#ifndef __FCS_H__
#define __FCS_H__

// #include "../imu_s/imu_s.h" // TODO: CHANGE THIS for RL_3...
#include "declarations.h"
#include "../pid/pid.h"
#include "../motor/motor.h"
#include "../rf/rf.h"

class FCS
{
    private:
        IMU * imu;
        Rx * rcv;
        /*
        m_1    m_2
          \    /
           \__/
           |/\|
           /  \
          /    \
        m_4    m_3
        */
        
        Motor * mot[4];
        PID_CTRL * pit_pid;
        PID_CTRL * rol_pid;
        PID_CTRL * yaw_pid;
        PID_CTRL * pow_pid;

        // uint8_t pry_sp[4]; // {power, yaw, pitch, roll}
        uint8_t mot_o[4];
        uint8_t pid_out[3]; // {yaw, pitch, roll}
    
    public:
        FCS();
        void attach_m(Motor * m_a, Motor * m_b, Motor * m_c, Motor * m_d);
        void attach_r(Rx * n_rcv);
        void attach_s(IMU * n_imu);
        void attach_ctrl(PID_CTRL * yaw, PID_CTRL * pit, PID_CTRL * rol);

        void setup_pids(float * y_coef, float * p_coef, float * r_coef);
        
        // Links the relavent pid ctrl w/ relavent sensor/motor io. using pointer operation.
        void link_ctrl();
        void calc_motor_o();
        void op_motors();
        
};


#endif

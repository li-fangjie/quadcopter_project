#ifndef __FCS_H__
#define __FCS_H__

// #include "../imu_s/imu_s.h" // TODO: CHANGE THIS for RL_3...
#include "declarations.h"
#include "../pid_t/pid_t.h"
#include "../motor/motor.h"
#include "../rf/rf.h"

typedef PID_CTRL<float, int, float> PID_YPR_CTRL;
typedef PID_CTRL<int16_t, int, float> PID_ACC_CTRl;

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
        PID_YPR_CTRL * pit_pid;
        PID_YPR_CTRL * rol_pid;
        PID_YPR_CTRL * yaw_pid;
        PID_ACC_CTRl * pow_pid;

        float pypr_sp[4]; // {power, yaw, pitch, roll} -> converts controller out to actual values.
        // uint8_t * pry_sp;
        int mot_o[4];
        // uint8_t pid_out[3]; // {yaw, pitch, roll}
    
    public:
        FCS();
        void attach_mot(Motor * m_a, Motor * m_b, Motor * m_c, Motor * m_d);
        void attach_mot(Motor * m_s);
        void attach_rx(Rx * n_rcv);
        void attach_imu(IMU * n_imu);
        void attach_pid(PID_YPR_CTRL * yaw, 
                        PID_YPR_CTRL * pit, 
                        PID_YPR_CTRL * rol, 
                        PID_ACC_CTRl * pow);

        // Links the relavent pid ctrl w/ relavent sensor/motor io. using pointer operation.
        void setup_pid(float * y_coef, float * p_coef, float * r_coef, float * pow_coef);
        // updates the control by radio
        int rcv_ctrl();
        // converts controller input (0-255) to actual values.
        void map_ctrl();
        // updates the pid controls;
        void update_pid();
        // combines result from PIDS to actual outputs of the 
        void calc_motor_o();
        // actually operates the motors
        void op_motors();
        
};


#endif

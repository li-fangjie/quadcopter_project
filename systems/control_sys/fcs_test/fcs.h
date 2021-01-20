#ifndef __FCS_H__
#define __FCS_H__

// #include "../imu_s/imu_s.h" // TODO: CHANGE THIS for RL_3...
// #include "declarations.h"
#include "constants.h"
#include "pid_t.h"
#include "motor_t.h"
#include "rx.h"
#include "imu_t.h"

// #define UPDATE_T 0.001 // 1 kHz update rate

typedef PID_CTRL<float, float, float> PID_D_CTRL;
typedef PID_CTRL<float, int16_t, float> PID_V_CTRL;
typedef Motor<int16_t> Motor_i; 

class FCS
{
	private:
		IMU * imu;
		Rx * rx;
		/*
		m_1    m_2
			\    /
				\__/
				|/\|
				/  \
			/    \
		m_4    m_3
		*/
		// Positive Yaw is CCW from above;
		// Positive Pitch is to the front from back;
		// Positive Roll is to the left from back;
		Motor_i * mots;
		PID_D_CTRL * d_pids;
		PID_V_CTRL * v_pids;

		float pypr_sp[4]; // {power, yaw, pitch, roll} -> converts controller out to actual values.
		// uint8_t * pry_sp;
		float outer_pid_o[2];
		int16_t inner_pid_o[4];
		uint8_t mot_o[4];
		float * fl_dat; // {yaw, pitch, roll, acc_z}
		// uint8_t pid_out[3]; // {yaw, pitch, roll}
		uint8_t ctrl_mode; // 1: cascade mode. 2: rate mode.
	
	public:
		FCS();
		void attach_mot(Motor_i * m_a, Motor_i * m_b, Motor_i * m_c, Motor_i * m_d);
		void attach_mot(Motor_i * m_s);
		void attach_rx(Rx * n_rcv);
		void attach_imu(IMU * n_imu);
		void attach_pid(PID_D_CTRL * outer_pids, PID_V_CTRL * inner_pids);

		void activate_pid();
		void config_pid();

		// Links the relavent pid ctrl w/ relavent sensor/motor io. using pointer operation.
		void setup_pid(float * outer_coefs[3], float * inner_coef[3]);
		// updates the control by radio
		int update_imu();
		int rcv_ctrl();
		// converts controller input (0-255) to actual values.
		void map_ctrl();
		// updates the pid controls;
		void update_pid();
		// combines result from PIDS to actual outputs of the 
		void map_motors_o();
		// actually operates the motors
		void op_motors();
		float * get_pypr_sp();

		uint8_t set_ctrl_mode(uint8_t mode, float * inner_coefs[3]);
};

#endif

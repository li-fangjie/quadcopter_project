#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/***************** GENERAL CONFIG PARAMETERS *****************/
#define UPDATE_T_MS 10 //ms, 100Hz update rate.
#define UPDATE_T 0.01 //s

/***************** IMU CONFIG PARAMETERS *****************/
#define MPU_ADDR 0x68

#define RAW_MAX 32767
#define RAW_MIN -32768

#define CALIB_CT 1000
#define COMP_COEF 0.98

#define BUF_TO_ANG 0.0076296273689992981 // = 1 / (2^(15)-1) * 250
#define BTA_UT 0.0000762962736899930 // = BUF_TO_ANG * UPDATE_T

#define RTD_CCF 1.1459155902616465// = RAD_TO_DEG * (1 - COMP_COEF)

#define MOV_AVG_CT 3
#define MA_O_COEF 0.66666666666666667 // = (1 - MOV_AVG_CT) / MOV_AVG_CT = 2 / 3
#define MA_N_COEF 33.333333333333333 // = 1 / UPDATE_T / MOV_AVG_CT = 1 / 0.01 /3
#define MA_N_Y_COEF 0.002543209122999766 // = BUF_TO_ANG / MOV_AVG_CT = 

/***************** PID CONFIG PARAMETERS *****************/

#define YPR_SP_COEF 0.0390625 // ±10˚ / 256
#define Y_DO_COEF 0.0390625 // ±10 / 226
/*
#define YAW_SP_COEF 0.078125 // 20˚ / 256
#define PITCH_SP_COEF 0.078125
#define ROLL_SP_COEF 0.078125
*/
#define ACC_Z_SP_COEF 0.076640625 // 0.076640625

/***************** PID MISC CONFIG PARAMETERS *****************/
#define YAW_RESO 1

#define ACC_Z_O_MAG 20
#define YAW_O_MAG 20
#define PITCH_O_MAG 20
#define ROLL_O_MAG 20

/***************** RF CONFIG PARAMETERS *****************/
#define QUAD_ADDR {0, 0, 0, 0, 1, 0}
#define CTRL_ADDR {0, 0, 0, 0, 0, 0}
#define IS_AUTOACK 1
#define BUF_LEN 6
#define HEAD_LEN 2
#define BODY_LEN 4
#define ACK_LEN 6

#endif

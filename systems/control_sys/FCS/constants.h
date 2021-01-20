#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/***************** GENERAL CONFIG PARAMETERS *****************/
#define UPDATE_T_MS 5 //ms, 200Hz update rate.
#define UPDATE_T 0.005 //s
#define OUTER_P 5

#define MISC_UT_MS 100

#define D_PID_CT 2
#define V_PID_CT 3

#define B_MOT_COEF 1

/***************** IMU CONFIG PARAMETERS *****************/
#define MPU_ADDR 0x68
#define DLPF_CFG 3

#define RAW_MAX 32767
#define RAW_MIN -32768

#define CALIB_CT 200
#define DISCARD_CT 200
#define COMP_COEF 0.98

#define BUF_TO_ANG 0.0076296273689992981 // = 1 / (2^(15)-1) * 250
#define BTA_UT 0.00003814813684 // = BUF_TO_ANG * UPDATE_T = 0.0076296273689992981 * 0.005

#define RTD_CCF 1.1459155902616465// = RAD_TO_DEG * (1 - COMP_COEF)

#define MOV_AVG_CT 3
#define MA_O_COEF 0.66666666666666667 // = (1 - MOV_AVG_CT) / MOV_AVG_CT = 2 / 3
#define MA_N_COEF 33.3333333333333333 // = 1 / UPDATE_T / MOV_AVG_CT = 1 / 0.005 /3
#define MA_N_Y_COEF 0.002543209122999766 // = BUF_TO_ANG / MOV_AVG_CT = 

/***************** PID CONFIG PARAMETERS *****************/

// #define DYPR_SP_COEF 0.07843137255 // ±10˚ / 255
#define DYPR_SP_COEF 0.03921568627 // ±5˚ / 255
#define VYPR_SP_COEF 0.9803921569 // Angular Rate / 255 = ±100˚/s / 255
#define VY_SP_COEF 0.2352941176 // ±30/s / 255
/*
#define YAW_SP_COEF 0.078125 // 20˚ / 256
#define PITCH_SP_COEF 0.078125
#define ROLL_SP_COEF 0.078125
*/
#define ACC_Z_SP_COEF 0.076640625 // 0.076640625

/***************** PID OUTPUT CONFIG PARAMETERS *****************/
#define V_RESO 0.5
#define D_RESO 0.1

// #define ACC_Z_O_MAG 20
#define YAW_O_MAG 60
#define PITCH_O_MAG 60
#define ROLL_O_MAG 60

#define D_YAW_O_MAG 20
#define D_PIT_O_MAG 60
#define D_ROL_O_MAG 60

/***************** RF CONFIG PARAMETERS *****************/
#define QUAD_ADDR {0, 0, 0, 0, 1, 0}
#define CTRL_ADDR {0, 0, 0, 0, 0, 0}
#define IS_AUTOACK 1
#define BUF_LEN 6
#define HEAD_LEN 2
#define BODY_LEN 4
#define ACK_LEN 6

#define MAX_DELAY_T 400

/***************** MISC CONFIG PARAMETERS *****************/
#define BAT_MIN 373  // (3.7 / 2) / 5 * 1024 // OK idk what is happening w/ this...
#define BAT_VAL_PIN A6
#define BAT_LED_PIN A1

#define PWR_MAX 170
#define PWR_MIN 0
#endif

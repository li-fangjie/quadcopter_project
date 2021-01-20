#ifndef __IMU_T_H__
#define __IMU_T_H__

#define UBRRH
#include "Arduino.h"
#include "Wire.h"
#include "constants.h"


typedef enum
{
    SELF_TEST_X = 0x0D,
    SELF_TEST_Y,
    SELF_TEST_Z,
    SELF_TEST_A,

    SMPLRT_DIV = 0x19,
    CONFIG,
    GYRO_CONFIG,
    ACCEL_CONFIG,

    FIFO_EN = 23,
    I2C_MST_CTRL,

    I2C_MST_STATUS = 0x36,
    INT_PIN_CFG,
    INT_ENABLE,

    INT_STATUS = 0x3A,
    ACCEL_XOUT_H,
    ACCEL_XOUT_L,
    ACCEL_YOUT_H,
    ACCEL_YOUT_L,
    ACCEL_ZOUT_H,
    ACCEL_ZOUT_L,
    TEMP_OUT_H,
    TEMP_OUT_L,
    GYRO_XOUT_H,
    GYRO_XOUT_L,
    GYRO_YOUT_H,
    GYRO_YOUT_L,
    GYRO_ZOUT_H,
    GYRO_ZOUT_L,

    USER_CTRL = 0x6A,
    PWR_MGMT_1,
    PWR_MGMT_2
} REG_ADD;


struct offset_struct {
    float acc_pr[2];
    int32_t raw[6];
    offset_struct()
    : acc_pr{0}, raw{0}
    {}
};

class IMU
{
    private:
        const uint8_t addr;
        offset_struct offset;
        int16_t raw_data[6]; // {acc_x, acc_y, acc_z, yaw, pitch, roll}
        float Vypr[3]; // {pitch, roll}
        float Dypr[3]; // {pitch_velocity, roll_velocity}
        // float ypr_deg[3];

    public:
        IMU();

        int setup();

        uint8_t test_conn();

        void calibrate();

        void read_raw();

        void read_raw(int16_t buf[6]);

        void calc_ypr();

        // void update_val();

        void get_Dypr(float data_buf[2]);

        int16_t * get_raw();

        float * get_Dypr();

        void get_Vypr(float data_buf[2]);

        float * get_Vypr();

        void set_sample_rate(uint8_t rate_div);
        
        // 0, 1, 2, 3 corrspond to: 
        // ±250, 500, 1000, 2000˚/s & ±2, 4, 8, 16g
        void set_range(uint8_t gyro, uint8_t acc);

        void power_up();

/******************** REGISTER OPERATIONRS ********************/
        void write_reg(uint8_t addr, uint8_t val);

        // bit is a bit mask
        void write_reg_bit(uint8_t addr, uint8_t bit, uint8_t val); 

        uint8_t read_reg(uint8_t addr);

        // bit is a bit mask
        uint8_t read_reg_bit(uint8_t addr, uint8_t bit); 

        void burst_read_reg(uint8_t start_addr, uint8_t * buf, uint8_t len);
};
#endif

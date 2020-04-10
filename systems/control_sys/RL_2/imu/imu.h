#ifndef __IMU_H__
#define __IMU_H__

#include "Arduino.h"
#include "I2Cdev.h"
// #include "MPU6050_6Axis_MotionApps20.h"
class Quaternion;
class VectorInt16;
class VectorFloat;
class MPU6050;
/*
A wrapper class of the MPU6050 library & I2Cdev.h
Currently only acts as a interface between FCS and those libraries.
*/
class IMU
{
    private:
        int dmp_data_ready;

        uint8_t int_status; // interrupt status
        uint16_t packet_size; // Size of each packet
        uint16_t fifo_byt_n; // # of bytes stored in fifo
        uint8_t fifo_buf[64]; // Buffer for fifo, should be 42?

        // containers needed for holding results...
        Quaternion * q;
        VectorInt16 * ac_raw; // raw measurements
        VectorInt16 * ac_real; // accel relative to self, gravity removed
        VectorInt16 * ac_world; // accel relative to intial world frame, gravity removed
        VectorFloat * ac_gravity; // accel due to gravity
        float euler[3]; // [psi, theta, phi]
        float ypr[3]; // [yaw, pitch roll]
        float ypr_deg[3];
        MPU6050 * mpu;

    public:
        int dmp_ready(); // The actual ISR method, changes dmp_data_ready
        int _setup(int interrupt_pin, void(*isr)());
        int setup(int interrupt_pin, void(*isr)());
        int setup(int interrupt_pin, void(*isr)(), int* offset);
        void calibrate();

        // read raw sensor value from FIFO into fifo_buf, 
        // returns 0 if succeeds, otherwise if fails.
        int read_raw(); 
        void calc_ypr(); // calculates ypr
        void calc_ac(); // calculates accel
        void update_val(); // updates ypr and accel.
        
        float * get_ypr();
        float * get_ypr_deg();
        VectorInt16 * get_p_aa_raw();
        VectorInt16 * get_p_aa_real();
        VectorInt16 * get_p_aa_world();
        int get_dmp_ready();
};

#endif
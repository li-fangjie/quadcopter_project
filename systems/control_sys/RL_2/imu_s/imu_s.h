#ifndef __IMU_S_H__
#define __IMU_S_H__
#include "Arduino.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "Wire.h"
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
        Quaternion q;
        VectorInt16 ac_raw; // raw measurements
        VectorInt16 ac_real; // accel relative to self, gravity removed
        VectorInt16 ac_world; // accel relative to intial world frame, gravity removed
        VectorFloat ac_gravity; // accel due to gravity
        
        float euler[3]; // [psi, theta, phi]
        float ypr[3]; // [yaw, pitch roll]
        float ypr_deg[3];
        
        float aw_z; // z acceleration for pid...
        MPU6050 mpu;

    public:
        int dmp_ready() // The actual ISR method, changes dmp_data_ready
        {
            dmp_data_ready = 1;
        }

        int _setup(int interrupt_pin, void(*isr)())
        // read raw sensor value from FIFO into fifo_buf, 
        // returns 0 if succeeds, otherwise if fails.
        {
            #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
                Wire.begin();
                TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
            #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
                Fastwire::setup(400, true);
            #endif

            mpu.initialize();
            int status = mpu.dmpInitialize();
            if (status != 0){
                int i = 0;
                while(i < 9 && status != 0){ 
                    // Tries reconnection 9 times if first try failed.
                    delay(500);
                    status = mpu.dmpInitialize();
                    ++i;
                }
                if(status != 0)
                    return status;
            }
            
            mpu.setDMPEnabled(true);
            
            pinMode(interrupt_pin, INPUT);
            attachInterrupt(digitalPinToInterrupt(interrupt_pin), isr, FALLING);
            // Some variable assignments...
            int_status = mpu.getIntStatus();
            packet_size = mpu.dmpGetFIFOPacketSize();
            
            delay(500); // to stabilise things?
            mpu.resetFIFO();
            mpu.getIntStatus();
            dmp_data_ready = 0;

            return status;
        }

        int setup(int interrupt_pin, void(*isr)(), int * mpu_offset)
        {
            _setup(interrupt_pin, isr);
            mpu.setXAccelOffset(mpu_offset[0]);
            mpu.setYAccelOffset(mpu_offset[1]);
            mpu.setZAccelOffset(mpu_offset[2]);
            mpu.setXGyroOffset(mpu_offset[3]);
            mpu.setYGyroOffset(mpu_offset[4]);
            mpu.setZGyroOffset(mpu_offset[5]);
        }

        int setup(int interrupt_pin, void(*isr)())
        {
            _setup(interrupt_pin, isr);
            // calibration
            delay(100);
            calibrate();

        }



        void calibrate()
        {
            mpu.CalibrateAccel(10);
            mpu.CalibrateGyro(10);
        }

        int read_raw()
        {
            dmp_data_ready = 0;
            fifo_byt_n = mpu.getFIFOCount();
            int_status = mpu.getIntStatus();
            // If incomplete packet is present/buffer overflows -> reset buffer
            if (fifo_byt_n % packet_size != 0 || fifo_byt_n > packet_size * 20) {
                int_status = mpu.getIntStatus();
                mpu.resetFIFO();
                int_status = mpu.getIntStatus();
                int err_code = (int_status & B10000)? 2 : 1;
                return err_code;
            } else {
                while(fifo_byt_n >= packet_size){
                    if (fifo_byt_n < packet_size) break;
                    mpu.getFIFOBytes(fifo_buf, packet_size);
                    fifo_byt_n -= packet_size;
                }
                int_status = mpu.getIntStatus();
            }
            return 0;
        }

        void calc_ypr()
        {
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);

            mpu.dmpGetYawPitchRoll(ypr, &q, &ac_gravity);
        }

        void calc_ac()
        {
            // Just.. go through everything needed.
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);
            
            mpu.dmpGetAccel(&ac_raw, fifo_buf);
            mpu.dmpGetLinearAccel(&ac_real, &ac_raw, & ac_gravity);
            mpu.dmpGetLinearAccelInWorld(&ac_world, &ac_real, &q);
            aw_z = ac_world.z;
        }

        void update_val()
        {
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);
            
            mpu.dmpGetYawPitchRoll(ypr, &q, &ac_gravity);

            mpu.dmpGetAccel(&ac_raw, fifo_buf);
            mpu.dmpGetLinearAccel(&ac_real, &ac_raw, & ac_gravity);
            mpu.dmpGetLinearAccelInWorld(&ac_world, &ac_real, &q);

            for(int i=0; i<3; ++i){
                ypr_deg[i] = ypr[i] * 180.0 / M_PI;
            }
        }

        float * get_ypr()
        {
            return ypr;
        }

        float * get_ypr_deg()
        {
            return ypr_deg;
        }

        VectorInt16 * get_p_aa_real()
        {
            return & ac_real;
        }

        VectorInt16 * get_p_aa_world()
        {
            return & ac_world;
        }

        VectorInt16 * get_p_aa_raw()
        {
            return & ac_raw;
        }

        float * get_awz()
        {
            return & aw_z;
        }

        int get_dmp_ready()
        {
            return dmp_data_ready;
        }

};
#endif

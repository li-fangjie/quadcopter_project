#ifndef __IMU_S_H__
#define __IMU_S_H__

#ifdef USE_IMU_S
#include "Arduino.h"
// #include "constants.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "Wire.h"

/***************** IMU CONFIG PARAMETERS *****************/
#define DD_MAX 2 // To avoid notable errors w/ significant jumps.
/*
#define ACC_MAX 32767
#define ACC_MIN -32768
#define G 9.80665
#define G2 19.6133
#define ACC_M_2G 0.0005985686819

#define MOV_AVG_CT 3
#define MOV_AVG_R 0.66666666666667 // 2/3

#define RAD_DEG_COEF 57.2957795131
// #define ACC_COEF 0.000001197137364 // * (1 / ACC_MAX) * G2 * UPDATE_T / MOV_AVG_CT (1 / 32767) * 19.6133 * 0.01 / 5
#define ACC_COEF 0.00000199522894
// #define DEG_COEF 20.0 // * (1 / UPDATE_T) / MOV_AVG_CT
#define DEG_COEF 33.3333333333
*/

#define MOV_AVG_CT 3
#define MA_O_COEF 0.66666666666666667 // = (1 - MOV_AVG_CT) / MOV_AVG_CT = 2 / 3
#define MA_N_COEF 33.3333333333333333 // = 1 / UPDATE_T / MOV_AVG_CT = 1 / 0.005 /3
#define MA_N_Y_COEF 0.002543209122999766 // = BUF_TO_ANG / MOV_AVG_CT = 


/*
A wrapper class of the MPU6050 library & I2Cdev.h
Currently only acts as a interface between FCS and those libraries.
*/
class IMU
{
    private:
        uint8_t dmp_data_ready;

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
        float Dypr[3]; // [yaw, pitch roll, acc_z_rate]
        float Vypr[3]; // {yaw_velocity, pitch_velocity, roll_velocity, acc_z_rate}
        // float ypr_deg[3];
        
        MPU6050 mpu;

    public:
        IMU()
        : int_status(0), euler{}, Dypr{}, Vypr{}
        {
        }
        
        void dmp_ready() // The actual ISR method, changes dmp_data_ready
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
                    delay(1000);
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
            
            delay(1000); // to stabilise things?
            mpu.resetFIFO();
            mpu.getIntStatus();
            dmp_data_ready = 0;
            mpu.setRate(0);
            mpu.setDLPFMode(MPU6050_DLPF_BW_42);

            return status;
        }

        int setup(int interrupt_pin, void(*isr)(), int * mpu_offset)
        {
            int status = _setup(interrupt_pin, isr);
            if(0 == status){
                mpu.setXAccelOffset(mpu_offset[0]);
                mpu.setYAccelOffset(mpu_offset[1]);
                mpu.setZAccelOffset(mpu_offset[2]);
                mpu.setXGyroOffset(mpu_offset[3]);
                mpu.setYGyroOffset(mpu_offset[4]);
                mpu.setZGyroOffset(mpu_offset[5]);
            }
            return status;
        }

        int setup(int interrupt_pin, void(*isr)())
        {
            int status = _setup(interrupt_pin, isr);
            if(0 == status){
                // calibration
                delay(100);
                calibrate();
            }
            return status;
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
            if (fifo_byt_n % packet_size != 0 || fifo_byt_n > packet_size * 40) {
                int_status = mpu.getIntStatus();
                mpu.resetFIFO();
                int_status = mpu.getIntStatus();
                int err_code = 3;
                if (int_status & B10000) {
                    err_code = 2;
                } else if (int_status & B1000) {
                    err_code = 1;
                }
                // Overflow / I2C_MST_INT
                return err_code;
            } else {
                while(fifo_byt_n >= packet_size){
                // if (fifo_byt_n >= packet_size){
                   if (fifo_byt_n < packet_size) break;
                   mpu.getFIFOBytes(fifo_buf, packet_size);
                   fifo_byt_n -= packet_size;
                }
                // update_val();
                // int_status = mpu.getIntStatus();
            }
            return 0;
        }

        int read_raw_fast()
        {
            dmp_data_ready = 0;
            int stat = mpu.getIntStatus();
            if(mpu.getFIFOCount() % packet_size != 0 || stat & B10000 || stat & B1000) {
                mpu.resetFIFO();
                return stat;
            }
            
            mpu.getFIFOBytes(fifo_buf, packet_size);
            return 0;
        }

        void calc_ypr(float * data_buf)
        {
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);
            mpu.dmpGetYawPitchRoll(data_buf, &q, &ac_gravity);
        }

        void calc_ypr()
        {
            float data_buf[3];
            float old_Dypr[3] = {Dypr[0], Dypr[1], Dypr[2]};
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);
            mpu.dmpGetYawPitchRoll(data_buf, &q, &ac_gravity);

            for (int i=0; i<3; i++) {
                Dypr[i] = data_buf[i] * RAD_TO_DEG;
                if(i == 1) {
                    Dypr[i] *= -1;
                }
                Vypr[i] = Vypr[i] * MA_O_COEF + (Dypr[i] - old_Dypr[i]) * MA_N_COEF;
            }
        }

        void calc_ac()
        {
            // Just.. go through everything needed.
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);
            
            mpu.dmpGetAccel(&ac_raw, fifo_buf);
            mpu.dmpGetLinearAccel(&ac_real, &ac_raw, & ac_gravity);
            mpu.dmpGetLinearAccelInWorld(&ac_world, &ac_real, &q);
        }
        /*
        void update_val()
        {
            float temp_ypr[3];
            VectorInt16 temp_acc;
            mpu.dmpGetQuaternion(&q, fifo_buf);
            mpu.dmpGetGravity(&ac_gravity, &q);
            mpu.dmpGetYawPitchRoll(temp_ypr, &q, &ac_gravity);

            mpu.dmpGetAccel(&ac_raw, fifo_buf);
            mpu.dmpGetLinearAccel(&ac_real, &ac_raw, & ac_gravity);
            mpu.dmpGetLinearAccelInWorld(&temp_acc, &ac_real, &q);
            for(int i=3; i>0; --i){ // bc ypr starts at ind 1
                temp_ypr[i-1] *= RAD_DEG_COEF;
                float DD = temp_ypr[i-1] - Vz_Dypr[i];
                if (abs(DD) < DD_MAX) {
                // Vzypr[i] += ((temp_ypr[i-1] - Vz_Dypr[i]) / UPDATE_T - Vzypr[i]) / MOV_AVG_CT;
                    Vzypr[i] = (DD) * DEG_COEF + Vzypr[i] * MOV_AVG_R;
                    Vz_Dypr[i] = temp_ypr[i-1];
                }
            }
            // Vzypr[0] += ((temp_acc.z - ac_world.z) * ACC_M_2G * UPDATE_T - Vzypr[0]) / MOV_AVG_CT;
            Vzypr[0] = (temp_acc.z - ac_world.z) * ACC_COEF + Vzypr[0] * MOV_AVG_R;
            Vz_Dypr[0] = Vzypr[0]; // copy over acc_z data.
        }
        */

        /*
        float * get_ypr()
        {
            return ypr;
        }

        float * get_ypr_deg()
        {
            return ypr_deg;
        }
        */

       float * get_Dypr()
       {
           return Dypr;
       }

       float * get_Vypr()
       {
           return Vypr;
       }

       int16_t * get_raw()
       {
           return NULL;
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
        
        uint8_t get_dmp_ready()
        {
            return dmp_data_ready;
        }

};

#endif
#endif
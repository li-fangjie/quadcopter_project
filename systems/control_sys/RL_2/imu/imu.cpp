#include "imu.h"

int IMU::check_dmp_ready()
{
    dmp_data_ready = 1;
}

int IMU::setup(int interrupt_pin, void(*isr)(), int* mpu_offset)
{
    interrupt_pin = 0;
    dmp_data_ready = 0;

    mpu.initialize();
    
    int status = mpu.dmpInitialize();
    if (status != 0){
        int i = 0;
        while(i < 9 || status == 0){ 
            // Tries reconnection 9 times if first try failed.
            status = mpu.dmpInitialize();
            ++i;
        }
        return status;
    }
    
    mpu.setXAccelOffset(mpu_offset[0]);
    mpu.setYAccelOffset(mpu_offset[1]);
    mpu.setZAccelOffset(mpu_offset[2]);
    mpu.setXGyroOffset(mpu_offset[3]);
    mpu.setYGyroOffset(mpu_offset[4]);
    mpu.setZGyroOffset(mpu_offset[5]);
    
    mpu.setDMPEnabled(true);
    
    pinMode(int_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(int_pin), isr, FALLING);
    mpu.resetFIFO();
    // Some variable assignments...
    int_status = mpu.getIntStatus();
    packet_size = mpu.dmpGetFIFOPacketSize();
    return status;
}

int IMU::read_raw()
{
    dmp_data_ready = 0;
    fifo_byt_n = mpu.getFIFOCount();
    int_status = mpu.getIntStatus();
    // If incomplete packet is present/buffer overflows -> reset buffer
    if (fifo_byt_n % packet_size != 0 || int_status & B10000) {
        mpu.resetFIFO();
        int err_code = (int_status & B10000)? 2 : 1;
        return err_code;
    } else {
        int r = mpu.GetCurrentFIFOPacket(fifo_buf, packet_size);
        return r;
    }
}

void IMU::calc_q_g()
{
    mpu.dmpGetQuaternion(&q, fifo_buf);
    mpu.dmpGetGravity(&ac_gravity, &q);
}

void IMU::calc_ypr()
{
    mpu.dmpGetQuaternion(&q, fifo_buf);
    mpu.dmpGetGravity(&ac_gravity, &q);

    mpu.dmpGetYawPitchRoll(ypr, &q, &ac_gravity);
}

void IMU::calc_ac()
{
    // Just.. go through everything needed.
    mpu.dmpGetQuaternion(&q, fifo_buf);
    mpu.dmpGetGravity(&ac_gravity, &q);
    
    mpu.dmpGetAccel(&ac_raw, fifo_buf);
    mpu.dmpGetLinearAccel(&ac_real, &ac_raw, & ac_gravity);
    mpu.dmpGetLinearAccelInWorld(&ac_world, &ac_real, &q);
}

void IMU::update_val()
{
    mpu.dmpGetQuaternion(&q, fifo_buf);
    mpu.dmpGetGravity(&ac_gravity, &q);
    
    mpu.dmpGetYawPitchRoll(ypr, &q, &ac_gravity);

    mpu.dmpGetAccel(&ac_raw, fifo_buf);
    mpu.dmpGetLinearAccel(&ac_real, &ac_raw, & ac_gravity);
    mpu.dmpGetLinearAccelInWorld(&ac_world, &ac_real, &q);
}

float * IMU::get_ypr()
{
    return ypr;
}

VectorInt16 * IMU::get_p_aa_real()
{
    return & ac_real;
}

VectorInt16 * IMU::get_p_aa_world()
{
    return & ac_world;
}

VectorInt16 * IMU::get_p_aa_raw()
{
    return & ac_raw;
}

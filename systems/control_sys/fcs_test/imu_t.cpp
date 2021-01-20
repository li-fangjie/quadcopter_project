#include "imu_t.h"
#include "Wire.h"

IMU::IMU()
: addr(MPU_ADDR), offset(), raw_data{0}, Vypr{0}, Dypr{0}
{}

int IMU::setup()
{
    Serial.println(F("***Setting up IMU unit...***"));
    if(0 != test_conn()) {
        // Serial.println("Connection to IMU failed");
        return 1;
    }
    power_up();
    set_range(0, 0); // Always set to ±250˚/s, ±2g
    // set_sample_rate(0);
    calibrate();
    Serial.println(F("***IMU Setup completed***"));
    return 0;
}

void IMU::calc_ypr()
{
    // for pitch(around y), the angle from acc should be x, z -> [0] / [1]
    for (int i=0; i<2; ++i) {
        // float base = sqrt((float)buf[2] * buf[2] + buf[2-i] * buf[2-i]);
        float prev_D = Dypr[i+1];
        
        Dypr[i+1] += (raw_data[i+4] - offset.raw[i+4]) * BTA_UT; 
        float acc_th = (atan2(raw_data[i], raw_data[2]) - offset.acc_pr[i]) * RTD_CCF; // RAD_TO_DEG * (1-COMP_COEF) 
        if (i == 0) acc_th *= -1;
        // NOTE: acc_th SIGN REVERSED for roll, in order to comply w/ MPU6050 orientation
        Dypr[i+1] = Dypr[i+1] * COMP_COEF + acc_th;

        // Vpr[i] += ((Dpr[i] - prev_D) / UPDATE_T - Vpr[i])/3;
        // Vypr[i+1] = Vypr[i+1] * MA_O_COEF + (Dypr[i+1] - prev_D) * MA_N_COEF;
    }
    for(int i=0; i<3; ++i) {
        Vypr[i] = Vypr[i] * MA_O_COEF + (raw_data[i+3] - offset.raw[i+3]) * MA_N_Y_COEF; 
    }
    // Vypr[0] = Vypr[0] * MA_O_COEF + (raw_data[3] - offset.raw[3]) * MA_N_Y_COEF; 
}

void IMU::read_raw()
{
    uint8_t temp_buf[14]; // buffer to hold all data, 14 bytes
    burst_read_reg(REG_ADD::ACCEL_XOUT_H, temp_buf, 14);
    /*
    for(int i=0; i<14; i+=2) { // 0 -> 12
      uint8_t temp = temp_buf[i]
      temp_buf[i] = temp_buf[i+1];
      temp_buf[i+1] = temp;
    }
    
    memcpy(&raw_data[0], &temp_buf[0], sizeof(int16_t)*3);
    memcpy(&raw_data[3], &temp_buf[8], sizeof(int16_t)*3);
    */ 
    // OK the above loop and memcpy is untested but should work
    // The loop flips highest/lowest bytes to fit int16_t, whilst memcpy does its thing.
    raw_data[0] = (temp_buf[0])<<8 | temp_buf[1];
    raw_data[1] = (temp_buf[2])<<8 | temp_buf[2];
    raw_data[2] = (temp_buf[4])<<8 | temp_buf[5];
    raw_data[5] = (temp_buf[8])<<8 | temp_buf[9]; // temp_buf[8]: x (roll) -> raw_data should be {yaw, pitch, roll}
    raw_data[4] = (temp_buf[10])<<8 | temp_buf[11]; // tmep_buf[10]: y (pitch)
    raw_data[3] = (temp_buf[12])<<8 | temp_buf[13]; // temp_buf[12]: z (yaw)
}

void IMU::read_raw(int16_t buf[6])
{
    uint8_t temp_buf[14]; // buffer to hold all data, 14 bytes
    burst_read_reg(REG_ADD::ACCEL_XOUT_H, temp_buf, 14);
    /*
    memcpy(&buf[0], &temp_buf[0], sizeof(int16_t)*3);
    memcpy(&buf[3], &temp_buf[8], sizeof(int16_t)*3);
    */
    buf[0] = (temp_buf[0])<<8 | temp_buf[1];
    buf[1] = (temp_buf[2])<<8 | temp_buf[2];
    buf[2] = (temp_buf[4])<<8 | temp_buf[5];
    buf[5] = (temp_buf[8])<<8 | temp_buf[9];
    buf[4] = (temp_buf[10])<<8 | temp_buf[11];
    buf[3] = (temp_buf[12])<<8 | temp_buf[13];
}

void IMU::calibrate()
{
    delay(1000);
    Serial.println(F("Calibrating IMU..."));
    Serial.print(F("Loop Count: "));
    Serial.println(CALIB_CT);
    int16_t temp[6];
    for(int i=0; i<CALIB_CT; ++i) {
        read_raw(temp);
        for(int j=0; j<6; ++j) {
            offset.raw[j] += temp[j];
        }
        delay(1);
        if(i % (100) == 0) {
            Serial.print(F("."));
        }
    }
    for (int i=0; i<6; ++i) {
        offset.raw[i] /= CALIB_CT;
        // Serial.println(offset.raw[i]);
    }
    offset.acc_pr[0] = atan2(offset.raw[0], offset.raw[2]);
    offset.acc_pr[1] = atan2(offset.raw[1], offset.raw[2]);
    delay(100);
    /*
    Serial.println(offset.acc_pr[0]);
    Serial.println(offset.acc_pr[1]);
    */
    Serial.println();
    Serial.println(F("Calibration Completed"));
}

void IMU::power_up()
{
    write_reg(REG_ADD::PWR_MGMT_1, 0);
}

void IMU::set_range(uint8_t gyro, uint8_t acc)
{
    write_reg(REG_ADD::GYRO_CONFIG, gyro<<3);
    write_reg(REG_ADD::ACCEL_CONFIG, acc<<3);
}

void IMU::set_sample_rate(uint8_t rate_div)
{
    write_reg(REG_ADD::SMPLRT_DIV, rate_div);
}

uint8_t IMU::test_conn()
{
    Wire.beginTransmission(addr);
    return Wire.endTransmission();
}

int16_t * IMU::get_raw()
{
    return raw_data;
}

void IMU::get_Dypr(float data_buf[2])
{
    memcpy(data_buf, Dypr, sizeof(float) * 2);
}

float * IMU::get_Dypr()
{
    return Dypr;
}

void IMU::get_Vypr(float data_buf[2])
{
    memcpy(data_buf, Vypr, sizeof(float) * 2);
}

float * IMU::get_Vypr()
{
    return Vypr;
}

/******************** REGISTER OPERATIONRS ********************/
void IMU::write_reg(uint8_t addr, uint8_t val)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(addr);
    Wire.write(val);
    Wire.endTransmission(false);
}

void IMU::write_reg_bit(uint8_t addr, uint8_t bit, uint8_t val)
{
    uint8_t org_val = read_reg(addr);
    org_val |= bit; // set the desired bit to high
    if (!val) { // If we actually want to clear the bit
        org_val ^= bit; // xor retains all other bit, except the masked bit
    }
    write_reg(addr, org_val);
}

uint8_t IMU::read_reg(uint8_t addr)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(addr);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 1, true);
    uint8_t reg_val = Wire.read();
    return reg_val;
}

uint8_t IMU::read_reg_bit(uint8_t addr, uint8_t bit)
{
    uint8_t reg_val = read_reg(addr);
    return reg_val & bit;
}

void IMU::burst_read_reg(uint8_t start_addr, uint8_t * buf, uint8_t len)
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(start_addr);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, (int)len, true);
    
    for(int i=0; i<len; ++i) {
        buf[i] = Wire.read();
    }
}

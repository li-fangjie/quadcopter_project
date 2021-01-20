#ifndef __RF_H__
#define __RF_H__
#define ARDUINO
#include "Arduino.h"
#include <SPI.h>
#include "RF24.h"

#define QUAD_ADDR "00001"
#define CTRL_ADDR "00000"
#define IS_AUTOACK 1
#define BUF_LEN 6
#define HEAD_LEN 2
#define ACK_BUF_LEN 4

class Tx
{
    private:
        RF24 * radio;
        uint8_t cur_pack_id;
        uint8_t buf[BUF_LEN];
        uint8_t addr[6];
        uint8_t targ_addr[6];
        uint8_t ack;
        

    public:
        Tx(RF24 * n_radio, uint8_t is_ack=1);
        int setup(uint8_t delay_t, uint8_t retry_t);
        int set_ctrl(uint8_t * ctrl_buf);
        int send_ctrl();
        int send_ctrl(float * ack_buf, uint8_t ack_len=ACK_BUF_LEN);
        int send_buf(uint8_t * s_buf, float* ack_buf, uint8_t size=BUF_LEN, uint8_t ack_len=ACK_BUF_LEN);
        uint8_t * get_buf();
};

class Rx
{
    private:
        RF24 * radio;
        uint8_t cur_pack_id;
        uint8_t buf[6];
        uint8_t addr[6];
        uint8_t targ_addr[6];
        uint8_t ack;
        // float f_buf[4]; // A lazy way to load 

    public:
        Rx(RF24 * radio, uint8_t is_ack=1);
        int setup();
        int update_ctrl();
        int update_ctrl(float * ack_buf, int len);
        uint8_t * get_buf();
        // float * get_f_buf();
};

#endif

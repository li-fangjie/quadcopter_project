#ifndef __RF_H__
#define __RF_H__

#ifndef ARDUINO
  #define ARDUINO 100
#endif
#define UBRRH
#include "Arduino.h"
#include "SPI.h"
#include "RF24.h"
#include "constants.h"

class Tx
{
    private:
        RF24 * radio;
        uint8_t cur_pack_id;
        uint8_t addr[6];
        uint8_t targ_addr[6];
        uint8_t * ctrl_buf;
        float * ack_buf;
        uint8_t ack;
        

    public:
        Tx(RF24 * n_radio);
        int setup(uint8_t delay_t, uint8_t retry_t, uint8_t * n_buf=NULL, float * n_ack_buf=NULL ,uint8_t is_ack=1);
        // int setup(uint8_t delay_t, uint8_t retry_t, uint8_t is_ack=1);
        void set_ctrl(uint8_t * ctrl_buf);
        // int send_ctrl();
        int send_ctrl(uint8_t * loc_ctrl_buf=NULL, float * loc_ack_buf=NULL);
        int send_buf(uint8_t * loc_ctrl_buf, float* loc_ack_buf=NULL);
        uint8_t * get_ctrl_buf();
};

/*
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
        int update_ctrl(float * loc_ack_buf, int len=ACK_LEN);
        uint8_t * get_buf();
        uint8_t * get_buf_body();
        // float * get_f_buf();
};
*/

#endif

#include "tx.h"
// #include "Arduino.h"


Tx::Tx(RF24 * n_radio)
: radio(n_radio), cur_pack_id(0), addr CTRL_ADDR, targ_addr QUAD_ADDR, ctrl_buf(NULL), ack_buf(NULL)
{
}

int Tx::setup(uint8_t delay_t, uint8_t retry_t, uint8_t * n_buf, float * n_ack_buf, uint8_t is_ack)
{
    // int result = setup(delay_t, retry_t, is_ack);
    ack = is_ack;
    radio -> begin();
    if (ack){
        radio -> enableAckPayload();
        radio -> enableDynamicPayloads();
    }
    radio -> setAutoAck(ack);
    radio -> openWritingPipe(targ_addr);
    radio -> openReadingPipe(1, addr);
    radio -> setRetries(delay_t, retry_t);

    radio -> startListening();
    if (!(radio -> isChipConnected()) || radio -> failureDetected) {
        return 1;
    }
    ctrl_buf = n_buf;
    ack_buf = n_ack_buf;
    return 0;
}
/*
int Tx::setup(uint8_t delay_t, uint8_t retry_t, uint8_t is_ack)
{
    radio -> begin();
    if (ack){
        radio -> enableAckPayload();
        radio -> enableDynamicPayloads();
    }
    radio -> setAutoAck(ack);
    radio -> openWritingPipe(targ_addr);
    radio -> openReadingPipe(1, addr);
    radio -> setRetries(delay_t, retry_t);

    radio -> startListening();
    if (!(radio -> isChipConnected()) || radio -> failureDetected) {
        return 1;
    }
    ack = is_ack;
    return 0;
}
*/

void Tx::set_ctrl(uint8_t buf[BUF_LEN])
{
    ctrl_buf = buf;
}

/*
int Tx::send_ctrl(uint8_t * ctrl_buf)
{
    set_ctrl(ctrl_buf);
    return send_ctrl();
}
*/
/*
int Tx::send_ctrl()
{
    uint8_t buf[BUF_LEN];
    buf[0] = B00000000;
    buf[1] = cur_pack_id;
    memcpy(&buf[HEAD_LEN], ctrl_buf, BODY_LEN * sizeof(uint8_t));
    float temp_buf[ACK_LEN];
    if (!send_buf(buf, temp_buf)) {
        ++cur_pack_id;
        return 0;
    }
    return 1;
}
*/


int Tx::send_ctrl(uint8_t * loc_ctrl_buf, float * loc_ack_buf)
{
    uint8_t * cur_ctrl_buf;
    if(ctrl_buf == NULL && loc_ctrl_buf == NULL) {
        return 2;
    } else if(loc_ctrl_buf != NULL) {
        cur_ctrl_buf = loc_ctrl_buf;
    } else {
        cur_ctrl_buf = ctrl_buf;
    }

    uint8_t buf[BUF_LEN] = {0};
    buf[0] = B00000000;
    buf[1] = cur_pack_id;
    memcpy(&buf[HEAD_LEN], cur_ctrl_buf, BODY_LEN * sizeof(uint8_t));

    uint8_t send_result;
    if(ack_buf == NULL && loc_ack_buf == NULL) {
        send_result = send_buf(buf, NULL);
    } else if(loc_ack_buf != NULL) {
        send_result = send_buf(buf, loc_ack_buf);
        if (ack_buf != NULL) {
            memcpy(ack_buf, loc_ack_buf, ACK_LEN);
        }
    } else if(ack_buf != NULL) {
        send_result = send_buf(buf, ack_buf);
    }
    // set_ctrl(buf, ctrl_buf);
    if (send_result == 0) {
        ++cur_pack_id;
        return 0;
    }
    return 1;
}

int Tx::send_buf(uint8_t * loc_ctrl_buf, float * loc_ack_buf)
{
    radio -> stopListening();
    if(radio->write(loc_ctrl_buf, BUF_LEN)) {
        if(ack == 0) {
            return 0;
        } else if(radio->available()) {
            if(loc_ack_buf != NULL) {
                radio->read(loc_ack_buf, sizeof(float) * ACK_LEN);
            }
            return 0;
        } else {
            return 2; // ack received has no payload
        }
    } else {
        return 1; // ack not received, although autoack enabled
    }
}

uint8_t * Tx::get_ctrl_buf()
{
  return ctrl_buf;
}

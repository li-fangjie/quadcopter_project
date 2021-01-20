#include "Arduino.h"
#include "rf.h"

Tx::Tx(RF24 * n_radio, uint8_t is_ack=1)
: radio(n_radio), cur_pack_id(0), buf({0}), addr(CTRL_ADDR), targ_addr(QUAD_ADDR), ack(is_ack)
{
}

int Tx::setup(uint8_t delay_t, uint8_t retry_t)
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
    
    if (radio -> isChipConnected() || radio -> failureDetected) {
        return 1;
    }
    return 0;
}

int Tx::set_ctrl(uint8_t * ctrl_buf)
{
    memcpy(buf + 2, ctrl_buf, (BUF_LEN - HEAD_LEN) * sizeof(uint8_t));
}

/*
int Tx::send_ctrl(uint8_t * ctrl_buf)
{
    set_ctrl(ctrl_buf);
    return send_ctrl();
}
*/

int Tx::send_ctrl()
{
    float temp_buf[8];
    buf[0] = B00000000;
    buf[1] = cur_pack_id;
    if (!send_buf(buf, temp_buf, BUF_LEN, 8)) {
        ++cur_pack_id;
        return 0;
    }
    return 1;
}

int Tx::send_ctrl(float * ack_buf, uint8_t ack_len)
{
    buf[0] = B00000000;
    buf[1] = cur_pack_id;
    if (!send_buf(buf, ack_buf)) {
        ++cur_pack_id;
        return 0;
    }
    return 1;
}

int Tx::send_buf(uint8_t * s_buf, float * ack_buf, uint8_t size=BUF_LEN, uint8_t ack_size=ACK_BUF_LEN)
{
    radio -> stopListening();
    if(radio->write(s_buf, size)) {
        if(radio->available()) {
            radio->read(ack_buf, sizeof(float) * ack_size);
            return 0;
        } else {
            return 2; // ack received has no payload
        }
    } else {
        return 1; // ack not received, although autoack enabled
    }
}

uint8_t * Tx::get_buf()
{
    return buf;
}

Rx::Rx(RF24 * radio, uint8_t is_ack=1)
: radio(radio), cur_pack_id(0), buf({0}), addr(QUAD_ADDR), targ_addr(CTRL_ADDR), ack(is_ack)
{ 
}

int Rx::setup()
{
    radio -> begin();
    if (ack){
        radio -> enableAckPayload();
        radio -> enableDynamicPayloads();
    }
    radio -> setAutoAck(ack);
    radio -> openWritingPipe(targ_addr);
    radio -> openReadingPipe(1, addr);
    radio -> startListening();
    if (radio -> isChipConnected() || radio -> failureDetected) {
        return 1;
    }
    radio -> writeAckPayload(1, &cur_pack_id, sizeof(cur_pack_id));
    return 0;
}

int Rx::update_ctrl()
{
    if (radio -> available()) {
        radio -> read(buf, BUF_LEN);
        cur_pack_id = buf[1];
        radio -> writeAckPayload(1, &cur_pack_id, sizeof(cur_pack_id));
        return 0;
    }
    return 1;
}

int Rx::update_ctrl(float * ack_buf, int len)
{
    if (radio -> available()) {
        radio -> read(buf, BUF_LEN);
        cur_pack_id = buf[1];
        // radio -> writeAckPayload(1, &cur_pack_id, sizeof(cur_pack_id));
        radio -> writeAckPayload(1, ack_buf, sizeof(float) * len);
        return 0;
    }
    return 1;
}

// NOTE: This returns the pointer to BUF_LEN buffer.
uint8_t * Rx::get_buf()
{
    return buf;
}

/*
// NOTE: This returns the pointer to a LENGTH 4 float buffer...
float * Rx::get_f_buf()
{
    return f_buf;
}
*/
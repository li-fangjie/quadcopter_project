#include "rx.h"
Rx::Rx(RF24 * radio)
: radio(radio), cur_pack_id(0), buf{}, addr QUAD_ADDR, targ_addr CTRL_ADDR, last_ctrl_t(0)
{ 
}

int Rx::setup(uint8_t is_ack)
{
    
    ack = is_ack;
    pinMode(10, OUTPUT); // By setting SS pin as output, it will not be written hight in SPI lib.
    radio -> begin();
    // digitalWrite(10, LOW); // This is to prevent interference w/ the PWM function. -> TOBE TESTED.
    
    if (ack){
        radio -> enableAckPayload();
        radio -> enableDynamicPayloads();
    }
    radio -> setAutoAck(ack);
    
    radio -> openWritingPipe(targ_addr);
    radio -> openReadingPipe(1, addr);
    radio -> startListening();
    
    if (!radio -> isChipConnected() || radio -> failureDetected) {
        return 1;
    }
    
    radio -> writeAckPayload(1, &cur_pack_id, sizeof(cur_pack_id));
    digitalWrite(10, LOW);
    return 0;
}

int Rx::update_ctrl()
{
    if (radio -> available()) {
        radio -> read(buf, BUF_LEN);
        cur_pack_id = buf[1];
        radio -> writeAckPayload(1, &cur_pack_id, sizeof(cur_pack_id));
        last_ctrl_t = millis();
        return 0;
    }
    return 1;
}

int Rx::update_ctrl(float * ack_buf)
{
    if (radio -> available()) {
        radio -> read(buf, BUF_LEN);
        cur_pack_id = buf[1];
        // radio -> writeAckPayload(1, &cur_pack_id, sizeof(cur_pack_id));
        radio -> writeAckPayload(1, ack_buf, sizeof(float) * ACK_LEN);
        last_ctrl_t = millis();
        return 0;
    }
    return 1;
}

// NOTE: This returns the pointer to BUF_LEN buffer.
uint8_t * Rx::get_buf()
{
    return buf;
}

uint8_t * Rx::get_buf_body()
{
    return &buf[HEAD_LEN];
}

long Rx::get_last_ctrl_t()
{
    return last_ctrl_t;
}

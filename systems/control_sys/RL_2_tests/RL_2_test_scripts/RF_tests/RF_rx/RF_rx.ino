#define UBRRH
#include "Arduino.h"
#include "rf.h"
// #include "../../RF/rf.h"

RF24 radio(7, 8);
Rx receiver(&radio);
uint8_t * buf;

void setup()
{
    Serial.begin(115200);
    Serial.println(F("RADIO RECEIVING TEST"));
    if(!receiver.setup()){
        Serial.println(F("Radio setup Failed."));
    }
    buf = receiver.get_buf();
}

void loop()
{
    if (!receiver.update_ctrl()) {
        for(int i = 0; i<BUF_LEN; ++i) {
            Serial.print(buf[i]);
            Serial.print(F(" "));
        }
        Serial.println();
    }
    
}

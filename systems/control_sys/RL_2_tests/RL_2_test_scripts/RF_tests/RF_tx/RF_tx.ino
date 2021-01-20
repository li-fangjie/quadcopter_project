#define UBRRH
#include "Arduino.h"
#include "rf.h"

RF24 rx_radio(7, 8);
Tx transmitter(&rx_radio);
uint8_t * buf;
uint8_t ctrl[4] = {100, 200, 300, 400};
long cur = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println(F("RADIO TRANSMITTING TEST"));
    if(!transmitter.setup(5, 5)){
        Serial.println(F("Radio setup Failed."));
    }
    buf = transmitter.get_buf();
}

void loop()
{
    if (millis() - cur > 500){
        cur = millis();
        int result = transmitter.send_ctrl(ctrl);
        for(int i=0; i<6;i++) {
            Serial.println(buf[i]);
        }
        Serial.print(F("The package was sent "));
        if(!result) {
            Serial.println(F("Successfully"));
        } else {
            Serial.println(F("unsuccessfully"));
            Serial.print(F("The error code is: "));
            Serial.println(result);
        }
    }
    

}

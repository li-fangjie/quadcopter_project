#define UBRRH
#include "Arduino.h"
#include "tx.h"
#define P_OFFSET 0
#define P_COEF 170/255.0
#define DEBOUNCE_T 10
#define LED_PIN 4
#define PWR_PIN 2
/***************** CONTROLLER PINS PARAMETERS *****************/
uint8_t joystick_pins[4] = {A0, A1, A2, A3};

// int offset[6] = { 1142,     603,    1122,       7,     162,     -35};
int8_t cmd_offset[4] = {0};
uint8_t ctrl[BODY_LEN] = {0};
float ypr[ACK_LEN] = {0};
RF24 radio(7,8);
Tx tx(&radio);


void setup()
{
    Serial.begin(115200);
    pinMode(PWR_PIN, INPUT_PULLUP);
    // Serial.begin(9600);
    // Serial.println(F("***Setting up Tx radio...***"));
    if(tx.setup(5, 5, ctrl, ypr)) {
        Serial.println("setup failed");
        while(1);
    }
    // Serial.println(F("***Tx radio setup completed...***"));
    pinMode(LED_PIN, HIGH);
}
 
unsigned long timer = 0;
bool cur_pwr_state = true;
void loop()
{   
    update_offset(cmd_offset);
    cur_pwr_state = pwr_state(PWR_PIN, cur_pwr_state);
    if(cur_pwr_state) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
    if(millis() - timer > 100){
        timer = millis();
        read_pypr_inputs(ctrl, BODY_LEN, joystick_pins, cur_pwr_state);
        ctrl[1] = 0;
        if(ctrl[0] > 0) {
            apply_offset(ctrl, cmd_offset);
        }
        /*
        if((int8_t)ctrl[2] > 100) { 
            ctrl[2] = 40;
        }
        */
        /*
        for(int i=0; i<BODY_LEN; ++i) {
            if(i==0) {
                Serial.print((uint8_t)ctrl[i]);
            } else {
                Serial.print((int8_t)ctrl[i]);
            }
            Serial.print(", "); 
        }
        // Serial.println();
        */
        Serial.print((uint8_t)ctrl[0]); Serial.print(", ");

        // Serial.print((int8_t)ctrl[2]); Serial.print(", ");
        // Serial.print((int8_t)ctrl[3]); Serial.print(", ");
        if(!tx.send_ctrl()) {
            // Serial.println(millis() - timer);
            // Serial.print(F(" Yaw: "));
            Serial.print(ypr[0]);
            Serial.print(F(", "));
            Serial.print(ypr[1]);
            Serial.print(F(", "));
            Serial.print(ypr[2]);
            Serial.print(F(", ")); Serial.print(ypr[3]);
            Serial.print(F(", ")); Serial.print(ypr[4]);
            Serial.print(F(", ")); Serial.print(ypr[5]);
            // Serial.println(ypr[3]);
        }
        Serial.print(", "); Serial.print((int8_t)ctrl[1]);
        Serial.print(", "); Serial.print((int8_t)ctrl[2]);
        Serial.print(", "); Serial.println((int8_t)ctrl[3]); 
 
    }
}

void apply_offset(uint8_t * ctrl, int8_t * cmd_offset) {
    ctrl[0] = constrain(ctrl[0] + cmd_offset[0], 0, 255);
    for(int i=1; i<4; i++) {
        ctrl[i] = constrain( (int8_t)ctrl[i] + cmd_offset[i], -128, 127);
    }
}

void update_offset(int8_t cmd_offset[4]) {
    if (Serial.available()) {
        char cmd = Serial.read();
        switch (cmd)
        {
            case 'W':
                cmd_offset[2] += 3;
                break;
            case 'w':
                cmd_offset[2] += 1;
                break;
            case 'S':
                cmd_offset[2] -= 3;
                break;
            case 's':
                cmd_offset[2] -= 1;
                break;
            case 'A':
                cmd_offset[3] -= 3;
                break;
            case 'a':
                cmd_offset[3] -= 1;
                break;
            case 'D':
                cmd_offset[3] += 3;
                break;
            case 'd':
                cmd_offset[3] += 1;
                break;
            case 'E':
                cmd_offset[1] += 50;
                break;
            case 'e':
                cmd_offset[1] += 1;
                break;
            case 'Q':
                cmd_offset[1] -= 50;
                break;
            case 'q':
                cmd_offset[1] -= 1;
                break;
            case 'X':
                cmd_offset[0] -= 3;
                break;
            case 'x':
                cmd_offset[0] -= 1;
                break;
            case 'C':
                cmd_offset[0] += 3;
                break;
            case 'c':
                cmd_offset[0] += 1;
                break;
            default:
                break;
        } 
    }
}

bool pwr_state(int pin, bool cur_state) {
    static bool prev_val = true;
    static bool debounce_val = true;
    static long debounce_timer = 0;
    bool cur_val = false;
    if(digitalRead(pin) == HIGH) cur_val = true;
    if(cur_val != debounce_val) {
        debounce_val = cur_val;
        debounce_timer = millis();
    }
    if(millis() - debounce_timer > DEBOUNCE_T) {
        if(cur_val == true && prev_val == false) {
            // Serial.print("Pwr_state Changed to: "); Serial.println(!cur_state);
            // Serial.print(prev_val); Serial.print(" "); Serial.println(cur_val);
            prev_val = cur_val;
            return !cur_state;
        } else {
            prev_val = cur_val;
            return cur_state;
        }
    }
}
void read_pypr_inputs(uint8_t * buf, uint8_t buf_len, uint8_t * ana_pins, bool pwr_state)
{
    if(pwr_state) {
        // int offset[4] = {2, 1, 1, 2};
        for(int i=1; i<buf_len; ++i) {
            buf[i] = (int8_t)(map(analogRead(ana_pins[i]), 0, 1023, 0, 255) - 128);
        }
        buf[0] = map(analogRead(ana_pins[0]), 0, 1023, 0, 255);
        if(buf[0] < P_OFFSET) buf[0] = P_OFFSET;
        // Serial.println((int8_t)buf[0]);
        buf[0] = (buf[0] - P_OFFSET) * P_COEF;
        // buf[1] = map((int8_t)buf[1], -128, 127, 127, -128);
    } else {
        for(int i=0; i<buf_len; ++i) {
            buf[i] = 0;
        }
    }
}

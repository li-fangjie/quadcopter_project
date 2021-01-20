/*
   Dec 2014 - TMRh20 - Updated
   Derived from examples by J. Coliz <maniacbug@ymail.com>
*/
/**
 * Example for efficient call-response using ack-payloads 
 * 
 * This example continues to make use of all the normal functionality of the radios including 
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well. 
 * This allows very fast call-response communication, with the responding radio never having to 
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to 
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication. 
 */
 
#include <SPI.h>
#include "RF24.h"
#include<Wire.h>


#define COMP_COEF 0.98
#define CALIB_CT 1000
#define DT 10
#define DT_S 0.01
#define REG_MAX 32767 // 2^15-1
#define BUF_TO_ANG 0.007629627369

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t buf[6] = {0};
float offset[8] = {0};
float ypr[3] = {0};
float Vypr[3] = {0};
long timer=0;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
                                                                           // Topology
byte addresses[][6] = {"1Node","2Node"};              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // The role of the current running sketch

byte counter = 1;                                                          // A single byte to keep track of the data being sent back and forth


void setup(){

  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted_CallResponse"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
 
  // Setup and configure radio

  radio.begin();

  radio.enableAckPayload();                     // Allow optional ack payloads
  radio.enableDynamicPayloads();                // Ack payloads are dynamic payloads
  
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, but opposite addresses
    radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();                       // Start listening  
  
  radio.writeAckPayload(1,&counter,1);          // Pre-load an ack-paylod into the FIFO buffer for pipe 1
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(115200);
  calibrate(offset);
  delay(3000);
  //radio.printDetails();
}

void loop(void) {

  
/****************** Ping Out Role ***************************/

  if (role == role_ping_out){                               // Radio is in ping mode

    byte gotByte;                                           // Initialize a variable for the incoming response
    
    radio.stopListening();                                  // First, stop listening so we can talk.      
    Serial.print(F("Now sending "));                         // Use a simple byte counter as payload
    Serial.println(counter);
    
    unsigned long time = micros();                          // Record the current microsecond count   
                                                            
    if ( radio.write(&counter,1) ){                         // Send the counter variable to the other radio 
        if(!radio.available()){                             // If nothing in the buffer, we got an ack but it is blank
            Serial.print(F("Got blank response. round-trip delay: "));
            Serial.print(micros()-time);
            Serial.println(F(" microseconds"));     
        }else{      
            while(radio.available() ){                      // If an ack with payload was received
                radio.read( &gotByte, 1 );                  // Read it, and display the response time
                unsigned long timer = micros();
                
                Serial.print(F("Got response "));
                Serial.print(gotByte);
                Serial.print(F(" round-trip delay: "));
                Serial.print(timer-time);
                Serial.println(F(" microseconds"));
                counter++;                                  // Increment the counter variable
            }
        }
    
    }else{        Serial.println(F("Sending failed.")); }          // If no ack response, sending failed
    
    delay(1000);  // Try again later
  }


/****************** Pong Back Role ***************************/

  if ( role == role_pong_back ) {
    byte pipeNo, gotByte;                          // Declare variables for the pipe and the byte received
    while( radio.available(&pipeNo)){              // Read all available payloads
      radio.read( &gotByte, 1 );                   
                                                   // Since this is a call-response. Respond directly with an ack payload.
      gotByte += 1;                                // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio.writeAckPayload(pipeNo,&gotByte, 1 );  // This can be commented out to send empty payloads.
      Serial.print(F("Loaded next response "));
      Serial.println(gotByte);  
        Serial.print("Pitch = "); 
        Serial.println(ypr[1]);
        Serial.print(" | Roll = "); Serial.println(ypr[2]);
        Serial.print("Pitch V = "); 
        Serial.print(Vypr[1]);
        Serial.print(" | Roll V = "); 
        Serial.println(Vypr[2]);
   }
 }



/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = role_ping_out;  // Become the primary transmitter (ping out)
      counter = 1;
   }else
    if ( c == 'R' && role == role_ping_out ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = role_pong_back; // Become the primary receiver (pong back)
       radio.startListening();
       counter = 1;
       radio.writeAckPayload(1,&counter,1);
    }
  }

  if(millis()-timer>=DT) {
      timer = millis();
      read_raw(buf);
      calc_pr(buf, offset, ypr, Vypr);
  }
}



void read_raw(int16_t * buf) 
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    buf[0] = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    buf[1] = Wire.read()<<8|Wire.read();  
    buf[2] = Wire.read()<<8|Wire.read();  
    int16_t temp = Wire.read()<<8|Wire.read();
    buf[3] = Wire.read()<<8|Wire.read();
    buf[4] = Wire.read()<<8|Wire.read();
    buf[5] = Wire.read()<<8|Wire.read();
    
      /*
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      */
}



void calibrate(float * offset)
{
    delay(1500);
    Serial.println(F("Calibrating IMU..."));
    Serial.print(F("Loop Count: "));
    Serial.println(CALIB_CT);
    int16_t temp[6];
    for(int i=0; i<CALIB_CT; ++i) {
        read_raw(temp);
        for(int j=0; j<6; ++j) {
            offset[j] += temp[j];
        }
        delay(5);
        if(i % (CALIB_CT/10) == 0) {
            Serial.print(F("."));
        }
    }
    for (int i=0; i<6; ++i) {
        offset[i] /= CALIB_CT;
    }
    offset[6] = atan2(offset[0], offset[2]);
    offset[7] = atan2(offset[1], offset[2]);
    delay(100);
    Serial.println();
    Serial.println(F("Calibration Completed"));
}

void calc_pr(int16_t * buf, float * offset, float * ypr, float * V_ypr)
{
    // for pitch(around y), the angle from acc should be x, z -> [0] / [1]
    for (int i=1; i<3; ++i) {
        // float base = sqrt((float)buf[2] * buf[2] + buf[2-i] * buf[2-i]);
        float prev_D = ypr[i];
        ypr[i] += (buf[i+3] - offset[i+3]) * BUF_TO_ANG * DT_S;
        // if (abs(buf[i-1]) > 328) {
        float acc_th = (atan2(buf[i-1], buf[2]) - offset[i+5]) * RAD_TO_DEG;
        ypr[i] = ypr[i] * COMP_COEF + acc_th * (1-COMP_COEF);
        // }
        V_ypr[i] += ((ypr[i] - prev_D) / DT_S - V_ypr[i])/3;
    }
}


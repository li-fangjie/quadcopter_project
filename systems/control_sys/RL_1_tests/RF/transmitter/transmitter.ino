#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN         
long cur = 0;
const byte address[]  = "00001";
void setup() {
  radio.begin();                  //Starting the Wireless communication
  Serial.begin(115200);
  
  
  radio.setDataRate(RF24_250KBPS);
  // radio.enableAckPayload();                     // Allow optional ack payloads
  // radio.enableDynamicPayloads();                // Ack payloads are dynamic payloads
  radio.setAutoAck(true);
  radio.setRetries(5, 5);
  radio.setPALevel(RF24_PA_MAX);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  if(!radio.isChipConnected())
  {
    Serial.println("Failure detected!");
  }
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.stopListening();          //This sets the module as transmitter
}
void loop()
{
  /* TRANSMIT */
  if (millis() - cur >= 500){
    cur = millis();
    const char text[] = "AAA";
    int r = radio.write(&text, sizeof(text));                  //Sending the message to receiver
    Serial.println(r);
    if(radio.available()){
      char ack[2]  = {0};
      radio.read(ack, sizeof(ack));    //Reading the data
      Serial.println(ack[0]);
    }
  }
}

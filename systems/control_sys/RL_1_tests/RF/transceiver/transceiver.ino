#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN         
long cur = 1;
const byte address[][6] = {"1Node", "2Node"};     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
int ind[][2] = {{0, 1}, {1, 0}};
void setup() {
  radio.begin();                  //Starting the Wireless communication
  Serial.begin(115200);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address[ind[1][0]]); //Setting the address where we will send the data
  radio.openReadingPipe(0, address[ind[1][1]]);   //Setting the address at which we will receive the data
  radio.enableAckPayload();                     // Allow optional ack payloads
  radio.enableDynamicPayloads();                // Ack payloads are dynamic payloads
  radio.setAutoAck(true);
  radio.setRetries(5, 5);
  radio.setPALevel(RF24_PA_MAX);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  if(!radio.isChipConnected())
  {
    Serial.println("Failure detected!");
  }
  radio.startListening();
}
void loop()
{
  /* TRANSMIT */
  if (millis() - cur >= 500){
    cur = millis();
    radio.stopListening();          //This sets the module as transmitter
    const char text[] = "I AM HIGH";
    int r = radio.write(&text, sizeof(text));                  //Sending the message to receiver
    Serial.println(r);
    if(radio.available()){
      char ack[2]  = {0};
      radio.read(ack, sizeof(ack));    //Reading the data
      Serial.println(ack[0]);
    }
    radio.startListening();
  }
  /* RECEIVE */
  if (radio.available())              //Looking for the data.
  {
    char rec[32] = {0};                 //Saving the incoming data
    char ack[1] = {9};
    radio.read(rec, 10);    //Reading the data
    radio.writeAckPayload(0, ack, 1);
    Serial.println(rec);
    // delay(5);
  }
}

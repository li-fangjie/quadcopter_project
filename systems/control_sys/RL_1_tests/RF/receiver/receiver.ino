#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(115200);
  radio.begin();
  
  
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setPALevel(RF24_PA_MAX);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  Serial.println("Testing for RF");
  if(!radio.isChipConnected())
  {
    Serial.println("Failure detected!");
  }
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.startListening();
}
void loop()
{
  if (radio.available())              //Looking for the data.
  {
    char text[32] = {0};                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    Serial.println(text);
    // delay(5);
  }
} 

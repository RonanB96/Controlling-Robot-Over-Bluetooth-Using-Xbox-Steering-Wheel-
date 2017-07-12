#include<SoftwareSerial.h>

SoftwareSerial BTSerial(6, 7); //RX|TX
//SoftwareSerial BTSerial(11, 10); //RX|TX


void setup(){
  Serial.begin(9600);
  BTSerial.begin(9600); // default baud rate
  BTSerial.println("AT");
}


void loop(){
  //read from the HM-10 and print in the Serial
  if(BTSerial.available())
    Serial.write(BTSerial.read());
    
  //read from the Serial and print to the HM-10
  if(Serial.available())
    BTSerial.write(Serial.read());
}

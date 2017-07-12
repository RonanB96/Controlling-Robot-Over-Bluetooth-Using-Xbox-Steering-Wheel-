//
// BLE_Car.ino
// This code control a robot car which is connected to a steering
// wheel over bluetooth which controls the output of the motors
// Instructables Post: https://www.instructables.com/id/Controlling-Robot-Over-Bluetooth-Using-Xbox-Steeri/
// Blog: http://www.roboroblog.wordpress.com
// Created By Ronan Byrne
// Last Updated 14/09/2016
//

#include<SoftwareSerial.h>
 
SoftwareSerial BTSerial(3, 2); //RX|TX

// Define Pins
int LMF = A5;
int LMR = A4;
int RMF = 7;
int RMR = 8;
int RS = 5;
int LS = 6;

// Define Variables
String message;
boolean Fdirection;
float wheel, rightPed, leftPed, Speed;

void setup() {
  Serial.begin(57600);
  BTSerial.begin(57600);

  pinMode(LMF, OUTPUT);
  pinMode(LMR, OUTPUT);
  pinMode(RMF, OUTPUT);
  pinMode(RMR, OUTPUT);
  pinMode(RS, OUTPUT);
  pinMode(LS, OUTPUT);
  Serial.println("AT");
  delay(1000);
  BTSerial.println("AT");

  // Set all outputs to off
  digitalWrite(LMF, LOW);
  digitalWrite(RMF, LOW);
  digitalWrite(LMR, LOW);
  digitalWrite(RMR, LOW);
  digitalWrite(RS, LOW);
  digitalWrite(LS, LOW);
}

void loop() {
  // Read from the HM-10
  if (BTSerial.available()) {
    // Parse the three floats
    wheel = BTSerial.parseFloat();
    leftPed = BTSerial.parseFloat();
    rightPed = BTSerial.parseFloat();
    message = String(wheel) + "," + String(leftPed) + "," +
              String(rightPed);
    //Serial.println(message);

    // Calculate the spped by the difference between the pedals
    Speed = ((rightPed - leftPed) / 5.0) * 255;
    if (Speed != 0) { // If speed is not zero, porcess data
      // If Speed is postive, the direction is forward
      if (Speed > 0.0) Fdirection = true;
      // if negative, the direction is reverse
      else {
        Fdirection = false;
        Speed = -Speed;
      }
      // Between 2.4V and 2.6V, the wheel is centered
      if (wheel > 2.6) {
        analogWrite(RS, Speed);
        analogWrite(LS, (1 - (wheel - 2.5) / 2.5)*Speed);
      }
      else if ( wheel < 2.4) {
        analogWrite(LS, Speed);
        analogWrite(RS, (wheel / 2.5)*Speed);
      }
      else {
        analogWrite(RS, Speed);
        analogWrite(LS, Speed);
      }


      if (Fdirection == true) {
        digitalWrite(LMF, HIGH);
        digitalWrite(RMF, HIGH);
        digitalWrite(LMR, LOW);
        digitalWrite(RMR, LOW);
      }
      else {
        digitalWrite(LMF, LOW);
        digitalWrite(RMF, LOW);
        digitalWrite(LMR, HIGH);
        digitalWrite(RMR, HIGH);
      }
    }
    else {
      digitalWrite(LMF, LOW);
      digitalWrite(RMF, LOW);
      digitalWrite(LMR, LOW);
      digitalWrite(RMR, LOW);
      analogWrite(RS, 0);
      analogWrite(LS, 0);
    }
  }
  //read from the Serial and print to the HM-10
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}

//
// BLE_Wheel.ino
// This code reads in inputs from an Xbox steering wheel such as the 
// the steering wheel, pedals and switches. These inputs are being sent over 
// bluetooth to a robot start which reacts to these inputs
// Instructables Post: https://www.instructables.com/id/Controlling-Robot-Over-Bluetooth-Using-Xbox-Steeri/
// Blog: http://www.roboroblog.wordpress.com
// Created by Ronan Byrne
// Last Updated 14/09/2016
//


#include<SoftwareSerial.h>
// Include timer library which can be found
// here http://www.doctormonk.com/search?q=timer
#include<Timer.h>

SoftwareSerial BTSerial(6, 7); // RX|TX
Timer t;

// Define Pins
int lPed = A2;
int rPed = A1;
int wheel = A0;
int Xb = 2;
int Xled = 4;

//Define Variables
String inString = "";
boolean Connecting = false;
boolean Connected = false;
boolean on = false;
String message = "";
int count = 0;
float reverse, forward, angle;

void setup() {
  Serial.begin(57600);         // Begin Serial Monitor at 57600 baudrate
  BTSerial.begin(57600);

  // Define INPUTS/OUTPUTS
  pinMode(lPed, INPUT);
  pinMode(rPed, INPUT);
  pinMode(wheel, INPUT);
  pinMode(Xb, INPUT);
  pinMode(Xled, OUTPUT);

  digitalWrite(Xled, LOW);    // Set Xbox LED to off

  // The function takeReading runs every 100ms 
  t.every(100, takeReading); 
  BTSerial.println("AT");
  delay(500);
}

void loop() {
  t.update();// update the timer
  if (digitalRead(Xb) == 0) { // If power is off and button pressed, turn on
    if (on == false) {
      on = true;
      digitalWrite(Xled, HIGH);  // Set Xbox LED to on
      Serial.println("Power On"); // Print "Power On"
      delay(10); // Small delay between serial prints
      BTSerial.println("AT");
    }
    else {
      on = false;
      digitalWrite(Xled, LOW);  // Set Xbox Led off
      Serial.println("Power Off"); // Print "Power Off"
    }
    delay(200);  // Delay 200ms to reduce chance of bouncing
  }
  // If Recieving data from the module
  if (BTSerial.available()) {
    inString = BTSerial.readStringUntil('\n'); // stop reading current data when a new line is found
    inString.trim(); // Trim whitespace 
    //Serial.println(inString); // Print data to serial monitor
    // If slave found, connect
    if (inString == "Devices Found 1"){
      BTSerial.println("AT+CONN1");
      Connecting =true;
    }
    // If connected, start reading and sending data
    else if(inString == "+Connected  0x001583007773"){
      Connected = true;
      Connecting = false;
      digitalWrite(Xled,HIGH);
    }
    // If disconnect, stop reading and sending data
    else if (inString == "+Disconnected") {
      Connected = false;
      digitalWrite(Xled, LOW);
    }
  }


  //read from the Serial and print to the HM-10
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}


void takeReading() {
  if (on == true) {
    if (Connected == true) {
      // Arduino has a 10bit ADC so to convert the bits to voltage the formula is (input_bits/2^10-1)*voltage_range
      angle = (float(analogRead(wheel)) / 1023) * 5.0;  // Voltage from Steering Wheel
      forward = (float(analogRead(rPed)) / 1023) * 5.0; // Voltage from Right Pedal
      reverse = (float(analogRead(lPed)) / 1023) * 5.0; // Voltage from Left Pedal
      // Create a string of the voltages
      message =  String(angle) + "," + String(reverse)
                 + "," + String(forward);
      BTSerial.println(message); // Print string
    }
    else if(Connecting == false && Connected == false )   {
      if (count == 30) {// Search for slave every 3 seconds
        SearchSlave();
        count = 0;
      }
      // flash led every half second when not connected
      else if (count == 5 || count == 10 || count == 15 || count == 20 || count ==25) {
        boolean ledState = digitalRead(Xled);
        digitalWrite(Xled, !ledState);
      }
      count++;
    }
  }
}


void SearchSlave() {
    boolean ledState = digitalRead(Xled);
    digitalWrite(Xled, !ledState);
    BTSerial.println("AT+INQ");
}

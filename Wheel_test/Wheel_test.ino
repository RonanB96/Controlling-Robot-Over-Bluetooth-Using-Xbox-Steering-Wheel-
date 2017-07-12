//
// Test code for reading in the analog voltages from an Xbox Steering Wheel
// and displaying these inputs on the serial monitor
// Instructables Post: https://www.instructables.com/id/Controlling-Robot-Over-Bluetooth-Using-Xbox-Steeri/
// Blog: http://www.roboroblog.wordpress.com
// Created by Ronan Byrne
// Last Updated 21/08/2016
//


// Define Pins
int lPed = A2;
int rPed = A1;
int wheel = A0;
int Xb = 2;
int Xled = 4;


//Define Variables
boolean on = false;
String message = "";
float reverse, forward, angle;


void setup() {
  Serial.begin(9600);         // Begin Serial Monitor at 9600 baudrate
  Serial.println("Started");  // Print "Started"
  // Define INPUTS/OUTPUTS
  pinMode(lPed, INPUT);
  pinMode(rPed, INPUT);
  pinMode(wheel, INPUT);
  pinMode(Xb, INPUT);
  pinMode(Xled, OUTPUT);


  digitalWrite(Xled, LOW);    // Set Xbox LED to off
}


void loop() {
  if (digitalRead(Xb) == 0 && on == false) { // If power is off and button pressed, turn on
    on = true;
    digitalWrite(Xled, HIGH);  // Set Xbox LED to on
    Serial.println("Power On"); // Print "Power On"
    delay(200);                 // Delay 200ms to reduce chance of bouncing
  }
  else if (on == true) {
    // Arduino has a 10bit ADC so to convert the bits to voltage the formula is (input_bits/2^10-1)*voltage_range
    angle = (float(analogRead(wheel)) / 1023) * 5.0;  // Voltage from Steering Wheel
    forward = (float(analogRead(rPed)) / 1023) * 5.0; // Voltage from Right Pedal
    reverse = (float(analogRead(lPed)) / 1023) * 5.0; // Voltage from Left Pedal
    // Create a string of the voltages
    message = "Steering Wheel: " + String(angle) + ", Left Pedal: " + String(reverse)
              + ", Right Pedal: " + String(forward);
    Serial.println(message); // Print string


    if (digitalRead(Xb) == 0) { // If Xbox button is pressed, power off
      on = false;
      digitalWrite(Xled, LOW);  // Set Xbox Led off
      Serial.println("Power Off"); // Print "Power Off"
      delay(200);
    }
    delay(500);   // Delay 500ms between readings
  }
}

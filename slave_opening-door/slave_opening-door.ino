#include <Wire.h>

// DEFINES
#define DEBUG
#define SLAVE_ADDR 9

// CONSTANTS
// Pin driven low to release a lock
const byte lockPin = A0;
// Button pin to relase lock
const byte buttonPin = 3;
// Button State
byte buttonState = HIGH;
// State of lock to send in answer
byte lockState = 0; 


void setup() {
  // put your setup code here, to run once:
  // Intitalise buttonPin
  pinMode(buttonPin, INPUT);

  // Set lockPin as output and secure Lock
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, HIGH);

  // initialise I2C Communication declaring it as Slave
  Wire.begin(SLAVE_ADDR);

  // Funciton to run wjen data requested by master
  Wire.onRequest(requestEvent);

  #ifdef DEBUG
    // Open communication
    Serial.begin(9600);
    Serial.println(F("Serial Comunication started"));
  #endif 
}

void requestEvent() {
  Wire.write(lockState);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    digitalWrite(lockPin, LOW);
    lockState = 1;
    #ifdef DEBUG
      Serial.println(F("Button Pressed"));
    #endif 
  }
  else if (buttonState == HIGH) {
    lockState = 0;
    digitalWrite(lockPin, HIGH);
  }
  delay(20);
}

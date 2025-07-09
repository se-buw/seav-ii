/* SteeringControl.h
   Encapsulates bang-bang steering control for HC-89 analog sensor.
   Usage:
     #include "SteeringControl.h"
     void setup() {
       Serial.begin(115200);
       initSteering();
       // prompt and set initial target
       Serial.println("Center wheels & press any key");
       while (!Serial.available()); Serial.read();
       setSteerTarget(CENTER_RAW);
     }
     void loop() {
       if (Serial.available()) {
         char c = Serial.read();
         switch (c) {
           case 'L': case 'l': setSteerTarget(LEFT_RAW); break;
           case 'C': case 'c': setSteerTarget(CENTER_RAW); break;
           case 'R': case 'r': setSteerTarget(RIGHT_RAW); break;
         }
       }
       updateSteering();
       // ... other loop logic ...
     }
*/

#ifndef STEERING_CONTROL_H
#define STEERING_CONTROL_H

#include <Arduino.h>

// Pin definitions
static const uint8_t STEER_ANALOG_PIN = A0;
static const uint8_t STEER_DIR_PIN    = 9;
static const uint8_t STEER_PWM_PIN    = 10;

// Calibration constants (raw analog values)
static const int LEFT_RAW   = 778;
static const int CENTER_RAW = 400;
static const int RIGHT_RAW  =  30;

// Control parameters
static const int STEER_DEADZONE = 10;
static const int STEER_SPEED    = 200; // PWM 0-255

// Current target
static int _steerTarget = CENTER_RAW;

// Initialize pins
inline void initSteering() {
  pinMode(STEER_ANALOG_PIN, INPUT);
  pinMode(STEER_DIR_PIN,   OUTPUT);
  pinMode(STEER_PWM_PIN,   OUTPUT);
  analogWrite(STEER_PWM_PIN, 0);
}

// Set desired steering target
inline void setSteerTarget(int raw) {
  _steerTarget = raw;
  Serial.print("Steer target set to ");
  Serial.println(raw);
}

// Bang-bang update, call each loop
inline void updateSteering() {
  int val = analogRead(STEER_ANALOG_PIN);
  int err = _steerTarget - val;
  if (abs(err) <= STEER_DEADZONE) {
    analogWrite(STEER_PWM_PIN, 0);
  } else {
    digitalWrite(STEER_DIR_PIN, (val < _steerTarget) ? HIGH : LOW);
    analogWrite(STEER_PWM_PIN, STEER_SPEED);
  }
}

#endif // STEERING_CONTROL_H

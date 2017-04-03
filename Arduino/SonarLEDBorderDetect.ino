#include <NewPing.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>


// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1800 // 

#define LED 13
#define FORWARD_SPEED 400
#define REVERSE_SPEED 200
#define TURN_SPEED 200
#define SONAR_RANGE 50 // in cm
#define NUM_SENSORS 6

const int echoPin = 3; // GRØNN KABEL 
const int triggerPin = 6; // GUL KABEL 
const int ledPin = A1; // ORANSJE KABEL
const int maxDistance = 30; // Limit for action in cm
long distance, time;
char val;

unsigned int sensor_values[NUM_SENSORS];
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); 
// Initialize "sonar" with trigger pin, echo pin and sonar range
NewPing sonar(triggerPin, echoPin, SONAR_RANGE);
// Tell Zumo that we have no emitter-pin
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); 


void setup() {
   pinMode(ledPin,OUTPUT);
   sensors.init();
   button.waitForButton();
   motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
   Serial.begin(9600);
}

void loop() {
//  if(Serial.available()) {
//    val = Serial.read();
//  }
//
//  if (val == 'd') {
//    checkDistance();
//  }
//  
//  if (val == 's') {
//    checkSensor();
//  }
//  
//  if (val == 'f') {
//    checkDistanceSensor();
//  }

  fightOn();
   
}


void fightOn() {
  checkDistance();
  sensors.read(sensor_values);
  
  // Hvis sensor 0 merker noe:
  if (sensor_values[0] < QTR_THRESHOLD) {
    sensor0();
  }
  // Hvis sensor 5 merker noe:
  else if (sensor_values[5] < QTR_THRESHOLD) {
    sensor5();
  }
  // Hvis INGEN sensor merker noe:
  else {
    if (!distance > 0) { // No object in front
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    } 
    else if (distance > 0) { // Object detected
      motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
    }
  }
}


void checkDistance() {
  // Read distance to obstacle, if any 
  time = sonar.ping();
  distance = sonar.convert_cm(time);
}

void sensor0() {
  // if leftmost sensor detects line, reverse and turn to the right
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(150);
  motors.setSpeeds(TURN_SPEED, -TURN_SPEED);   
  delay(300);
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED); 
  delay(200);
}

void sensor5() {
  // if rightmost sensor detects line, reverse and turn to the left
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  delay(150);
  motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
  delay(300);
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  delay(200);
}

void checkSensor() {
  // Read IR-sensors and check if border detected
  sensors.read(sensor_values);
  if (sensor_values[0] > QTR_THRESHOLD) {
    sensor0();
  }
  if (sensor_values[5] > QTR_THRESHOLD) {
    sensor5();
  }
  if (!(sensor_values[0] > QTR_THRESHOLD) and !(sensor_values[0] > QTR_THRESHOLD)) {
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
  }
}

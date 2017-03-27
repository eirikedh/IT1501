#include <NewPing.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>


// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1800 // 

#define LED 13
#define FORWARD_SPEED     100
#define SONAR_RANGE 50 // in cm
#define NUM_SENSORS 6

const int echoPin = 2; // GRØNN KABEL 
const int triggerPin = 3; // GUL KABEL 
const int ledPin = A1; // ORANSJE KABEL
const int maxDistance = 20; // Limit for action in cm
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
  if(Serial.available()) {
    val = Serial.read();
  }

  if (val == 'd') {
    checkDistance();
  }
  
  if (val == 's') {
    checkSensor();
  }
  
  if (val == 'a') {
    checkDistanceSensor();
  }
  
  //checkDistance();
  //checkSensor();
  //checkDistanceSensor();
   
}


void checkDistance() {
  // Read distance to obstacle, if any 
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  // check if inside range for action  
  if (!distance > 0) { // No object in front
    digitalWrite(ledPin, LOW); 
    motors.setSpeeds(-2*FORWARD_SPEED, 2*FORWARD_SPEED);
  } 
  else if (distance > 0) { // Object detected
    digitalWrite(ledPin,HIGH);
    // Drive towards object
    motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  }
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
    motors.setSpeeds(-2*FORWARD_SPEED, 2*FORWARD_SPEED);
  }
}


void checkDistanceSensor() {
  sensors.read(sensor_values);
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  // No object in front or sensor value detected
  if (!(distance > 0) and !(sensor_values[0] > QTR_THRESHOLD) and !(sensor_values[5] > QTR_THRESHOLD)) { 
    digitalWrite(ledPin, LOW); 
    motors.setSpeeds(-2*FORWARD_SPEED, 2*FORWARD_SPEED);
  } 
  // Object detected
  else if ((distance > 0) and !(sensor_values[0] > QTR_THRESHOLD) and !(sensor_values[5] > QTR_THRESHOLD)) { 
    digitalWrite(ledPin,HIGH);
    motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  }
  // Sensor_value[0] detects
  else if ((!distance > 0) and (sensor_values[0] > QTR_THRESHOLD) and (!sensor_values[5] > QTR_THRESHOLD)) {
    digitalWrite(ledPin, LOW); 
    sensor0();
  }
  // Sensor_value[5] detects
  else if ((!distance > 0) and (!sensor_values[0] > QTR_THRESHOLD) and (sensor_values[5] > QTR_THRESHOLD)) {
    digitalWrite(ledPin, LOW); 
    sensor5();  
  }
  else {
    digitalWrite(ledPin, LOW); 
    motors.setSpeeds(-2*FORWARD_SPEED, 2*FORWARD_SPEED);
  }

}

void sensor0() {
  // if leftmost sensor detects line, reverse and turn to the right
  motors.setSpeeds(-FORWARD_SPEED, -FORWARD_SPEED);
  delay(FORWARD_SPEED);
  motors.setSpeeds(FORWARD_SPEED, -FORWARD_SPEED);   
  delay(FORWARD_SPEED);
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED); 
}

void sensor5() {
  // if rightmost sensor detects line, reverse and turn to the left
  motors.setSpeeds(-FORWARD_SPEED, -FORWARD_SPEED);
  delay(FORWARD_SPEED);
  motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
  delay(FORWARD_SPEED);
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
}


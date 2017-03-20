#include <NewPing.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

#define LED 13

// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1800 // 
  
// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms
#define LEFT 1  // turn direction
#define RIGHT 2
#define SONAR_RANGE       30 // in cm

const int echoPin = 2; // GRØNN KABEL 
const int triggerPin = 3; // GUL KABEL 
const int ledPin = A1; // ORANSJE KABEL
const int maxDistance = 20; // Limit for action in cm


#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
// Initialize "sonar" with trigger pin, echo pin and max distance,
// and then servo, motor, button and IR-sensor array
NewPing sonar(triggerPin, echoPin, SONAR_RANGE);
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); 
// Tell Zumo that we have no emitter-pin
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); 
 
 
void setup() {
   pinMode(ledPin,OUTPUT);
   button.waitForButton();
   motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
}

void loop() {
  // Read distance to obstacle, if any 
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);

  // check if inside range for action
  if (! distance > 0) { // sonar gives zero when outside range
    // No object in front, turn around
    digitalWrite(ledPin,LOW); 
    motors.setSpeeds(-2*FORWARD_SPEED, 2*FORWARD_SPEED);
  } 
  
  else if (distance > 0) {
    // Object detected
    digitalWrite(ledPin,HIGH);
    // Drive towards object
    motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  }  
 
  // Read IR-sensors and check if border detected
  sensors.read(sensor_values);
  if (sensor_values[0] > QTR_THRESHOLD) {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    //turn(RIGHT);
  }
  
  else if (sensor_values[5] > QTR_THRESHOLD) {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    //turn(LEFT);
  }

}

void turn(int direction){ 
  // Move Zumo backwards, then turn in given direction,
  // and continue forward
  // Check wich direction to turn
  if (direction == RIGHT) {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  
  else { // turn left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }  
  
}


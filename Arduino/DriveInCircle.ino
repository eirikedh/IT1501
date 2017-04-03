#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

#define QTR_THRESHOLD  1800 // 
#define FORWARD_SPEED 400
#define TURN_SPEED 400

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);


void setup() {
  button.waitForButton();

  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  delay(350);
  motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
  delay(175);
  motors.setSpeeds(250, FORWARD_SPEED);
}

void loop() {
  
}

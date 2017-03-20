#include <NewPing.h>
#include <NewServo.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>

#define FORWARD_SPEED 150 //Motor speed Zumo
#define SONAR_RANGE 50  // in cm
const int echoPin = 2; // GRØNN KABEL
const int triggerPin = 3; // GUL KABEL
// Max distance (in cm)
const int maxDistance = 20; // Limit for action cm

const int ledPin=A1; // ORANSJE KABEL
const int servoPin = 6;
int degreesServo = 0;
int degreesStep = 30;


NewPing sonar(triggerPin, echoPin, SONAR_RANGE);  // make sonar and
NewServo myServo;                         // servo
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); 

void setup() {
   pinMode(ledPin,OUTPUT);
   myServo.attach(servoPin); 
   button.waitForButton(); // start when button pressed
   motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
   Serial.begin(9600);
}

void loop() {
  // Use sonar to detct objects within range and convert to distance
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  //
  Serial.println(distance);
  // check if inside range for action
  if (! distance > 0) { // sonar gives zero when outside range
    // Turn off LED and just go forward
    digitalWrite(ledPin,LOW); 
   motors.setSpeeds(-2*FORWARD_SPEED, 2*FORWARD_SPEED);
   } 
   else {
   // Light LED and turn slighly away from object detected
   // while turning servo forth and back in steps (0-180)
     digitalWrite(ledPin,HIGH);
     // Turn way from object by rotating left (obstacle)
     motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
   }
}

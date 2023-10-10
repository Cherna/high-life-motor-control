#include <AccelStepper.h>

// const int stepXPin = 2; //X.STEP
// const int dirXPin = 5; // X.DIR
// const int stepYPin = 3; //Y.STEP
// const int dirYPin = 6; // Y.DIR
// const int stepZPin = 4; //Z.STEP
// const int dirZPin = 7; // Z.DIR

#define enableMotorsPin 8

#define MotorInterfaceType 1

#define ZMotorDirPin 7
#define ZMotorStepPin 4

#define YMotorDirPin 6
#define YMotorStepPin 3

#define XMotorDirPin 5
#define XMotorStepPin 2

#define AMotorDirPin 13
#define AMotorStepPin 12

struct SpeedRange {
  int minSpeed;
  int maxSpeed;
};

SpeedRange motorSpeedRange = {20, 4000};

int xSpeedVal = motorSpeedRange.maxSpeed;
int xAccel = 80;

AccelStepper aStepper = AccelStepper(MotorInterfaceType, AMotorStepPin, AMotorDirPin);
AccelStepper xStepper = AccelStepper(MotorInterfaceType, XMotorStepPin, XMotorDirPin);
AccelStepper yStepper = AccelStepper(MotorInterfaceType, YMotorStepPin, YMotorDirPin);
AccelStepper zStepper = AccelStepper(MotorInterfaceType, ZMotorStepPin, ZMotorDirPin);

void enableMotors() {
  digitalWrite(enableMotorsPin, LOW);
}
void disableMotors() {
  digitalWrite(enableMotorsPin, HIGH);
}

void setup() {
 	Serial.begin(9600);

  pinMode(enableMotorsPin, OUTPUT);

  pinMode(AMotorDirPin, INPUT);
  pinMode(AMotorStepPin, OUTPUT);

  pinMode(XMotorDirPin, INPUT);
  pinMode(XMotorStepPin, OUTPUT);

  pinMode(YMotorDirPin, INPUT);
  pinMode(YMotorStepPin, OUTPUT);

  pinMode(ZMotorDirPin, INPUT);
  pinMode(ZMotorStepPin, OUTPUT);

  enableMotors();

  zStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  zStepper.setSpeed(xSpeedVal);
}

void loop() {
  zStepper.setSpeed(motorSpeedRange.maxSpeed);
  zStepper.runSpeed();
}


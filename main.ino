#include <AccelStepper.h>
#include <MIDI.h>

#define CHANNEL 1
#define MotorInterfaceType 1
unsigned long curMillis; // Variables for throttling
unsigned long prevMillis = 0; // Naming is not great

// This would be the most correct approach probably
// but I'm not feeling like learning classes in cpp at this point
// class StepperControl {
//   public:
//     StepperControl(int enablePin, int dirPin, int stepPin) {
      
//     }
// };

#define accelerationMillis 20
#define throttleMillis 500

MIDI_CREATE_DEFAULT_INSTANCE();

#define enableMotorsPin 8

#define ZMotorEnPin 2
#define ZMotorDirPin 3
#define ZMotorStepPin 4

#define YMotorEnPin 5
#define YMotorDirPin 6
#define YMotorStepPin 7

#define XMotorEnPin 8
#define XMotorDirPin 9
#define XMotorStepPin 10

#define AMotorEnPin 11
#define AMotorDirPin 12
#define AMotorStepPin 13

struct SpeedRange {
  int minSpeed;
  int maxSpeed;
};

SpeedRange motorSpeedRange = {20, 4000};

int speedVal = motorSpeedRange.maxSpeed;
int motAccel = 80;

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

int mapMIDIValueToSpeedRange(int value) {
  return map(value, 0, 128, motorSpeedRange.minSpeed, motorSpeedRange.maxSpeed);
}

bool mapMIDIValueToSpinDirection(int value) {
  if (value < 128/2) {
    return LOW;
  } else {
    return HIGH;
  }
}

void setup() {
 	MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);

  pinMode(enableMotorsPin, OUTPUT);

  pinMode(AMotorEnPin, INPUT);
  pinMode(AMotorDirPin, OUTPUT);
  pinMode(AMotorStepPin, OUTPUT);

  pinMode(XMotorEnPin, INPUT);
  pinMode(XMotorDirPin, OUTPUT);
  pinMode(XMotorStepPin, OUTPUT);

  pinMode(YMotorEnPin, INPUT);
  pinMode(YMotorDirPin, OUTPUT);
  pinMode(YMotorStepPin, OUTPUT);

  pinMode(ZMotorEnPin, INPUT);
  pinMode(ZMotorDirPin, OUTPUT);
  pinMode(ZMotorStepPin, OUTPUT);

  enableMotors();

  MIDI.setHandleControlChange(readMIDI);

  xStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  xStepper.setSpeed(speedVal);
  yStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  yStepper.setSpeed(speedVal);
  zStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  zStepper.setSpeed(speedVal);
  aStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  aStepper.setSpeed(speedVal);
}

void readMIDI(byte channel, byte number, byte value) {
  // Channel mapping
  // - ch1: xSpeed
  // - ch2: xDir
  // - ch3: ySpeed
  // - ch4: yDir
  // - ch5: zSpeed
  // - ch6: zDir
  // - ch7: aSpeed
  // - ch8: aDir
  if (number == 0) {
    xStepper.setSpeed(mapMIDIValueToSpeedRange(value));
  }
  if (number == 1) {
    digitalWrite(XMotorDirPin, mapMIDIValueToSpinDirection(value));
  }
}

void loop() {
  curMillis = millis();

  // Do something only every 'throttleMillis'
  if (curMillis - prevMillis >= throttleMillis) {
    prevMillis = curMillis;

    Serial.println(xStepper.speed());
    Serial.println(digitalRead(XMotorDirPin));
  }

  // Run all motors as long as they are enabled
  if (digitalRead(ZMotorEnPin)) {
    // zStepper.setSpeed(1000);
    zStepper.runSpeed();
  }
  if (digitalRead(XMotorEnPin)) {
    // xStepper.setSpeed(1000);
    xStepper.runSpeed();
  }
  if (digitalRead(YMotorEnPin)) {
    // yStepper.setSpeed(1000);
    yStepper.runSpeed();
  }
  if (digitalRead(AMotorEnPin)) {
    // aStepper.setSpeed(1000);
    aStepper.runSpeed();
  }

  MIDI.read();
}


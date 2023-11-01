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
int ZMotorDir = 0;
int ZMotorEn = 0;

#define YMotorEnPin 5
#define YMotorDirPin 6
#define YMotorStepPin 7
int YMotorDir = 0;
int YMotorEn = 0;

#define XMotorEnPin 8
#define XMotorDirPin 9
#define XMotorStepPin 10
int XMotorDir = 0;
int XMotorEn = 0;

#define AMotorEnPin 11
#define AMotorDirPin 12
#define AMotorStepPin 13
int AMotorDir = 0;
int AMotorEn = 0;

struct SpeedRange {
  int minSpeed;
  int maxSpeed;
};

SpeedRange motorSpeedRange = {1000, 6000};

int speedVal = motorSpeedRange.minSpeed;
int motAccel = 80;

AccelStepper aStepper = AccelStepper(MotorInterfaceType, AMotorStepPin, AMotorDirPin);
AccelStepper xStepper = AccelStepper(MotorInterfaceType, XMotorStepPin, XMotorDirPin);
AccelStepper yStepper = AccelStepper(MotorInterfaceType, YMotorStepPin, YMotorDirPin);
AccelStepper zStepper = AccelStepper(MotorInterfaceType, ZMotorStepPin, ZMotorDirPin);

int mapMIDIValueToSpeedRange(int value) {
  return map(value, 0, 128, motorSpeedRange.minSpeed, motorSpeedRange.maxSpeed);
}

bool mapMIDIValueToBool(int value) {
  if (value <= 128/2) {
    return 0;
  } else {
    return 1;
  }
}

void setup() {
 	MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);

  pinMode(AMotorEnPin, OUTPUT);
  pinMode(AMotorDirPin, OUTPUT);
  pinMode(AMotorStepPin, OUTPUT);

  pinMode(XMotorEnPin, OUTPUT);
  pinMode(XMotorDirPin, OUTPUT);
  pinMode(XMotorStepPin, OUTPUT);

  pinMode(YMotorEnPin, OUTPUT);
  pinMode(YMotorDirPin, OUTPUT);
  pinMode(YMotorStepPin, OUTPUT);

  pinMode(ZMotorEnPin, OUTPUT);
  pinMode(ZMotorDirPin, OUTPUT);
  pinMode(ZMotorStepPin, OUTPUT);

  xStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  xStepper.setSpeed(speedVal);
  yStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  yStepper.setSpeed(speedVal);
  zStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  zStepper.setSpeed(speedVal);
  aStepper.setMaxSpeed(motorSpeedRange.maxSpeed);
  aStepper.setSpeed(speedVal);

  MIDI.setHandleControlChange(readMIDI);
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
    XMotorDir = mapMIDIValueToBool(value);
  }
  if (number == 2) {
    yStepper.setSpeed(mapMIDIValueToSpeedRange(value));
  }
  if (number == 3) {
    YMotorDir = mapMIDIValueToBool(value);
  }
  if (number == 4) {
    zStepper.setSpeed(mapMIDIValueToSpeedRange(value));
  }
  if (number == 5) {
    ZMotorDir = mapMIDIValueToBool(value);
  }
  if (number == 6) {
    aStepper.setSpeed(mapMIDIValueToSpeedRange(value));
  }
  if (number == 7) {
    AMotorDir = mapMIDIValueToBool(value);
  }
  if (number == 8) {
    XMotorEn = mapMIDIValueToBool(value);
  }
  if (number == 9) {
    YMotorEn = mapMIDIValueToBool(value);
  }
  if (number == 10) {
    ZMotorEn = mapMIDIValueToBool(value);
  }
  if (number == 11) {
    AMotorEn = mapMIDIValueToBool(value);
  }
}

void loop() {
  curMillis = millis();
  MIDI.read();

  // Do something only every 'throttleMillis'
  if (curMillis - prevMillis >= throttleMillis) {
    prevMillis = curMillis;

    // Serial.println(zStepper.speed());
    // Serial.print("ZDir: ");
    // Serial.println(ZMotorDir);
    // Serial.print("ZEn: ");
    // Serial.println(ZMotorEn);
    // Serial.println(yStepper.speed());
    // Serial.print("YDir: ");
    // Serial.println(YMotorDir);
    // Serial.print("YEn: ");
    // Serial.println(YMotorEn);
  }

  // X Motor
  digitalWrite(XMotorEnPin, XMotorEn);
  if (XMotorEn) {
    digitalWrite(XMotorDirPin, XMotorDir);
    xStepper.runSpeed();
  }

  // Y Motor
  digitalWrite(YMotorEnPin, YMotorEn);
  if (YMotorEn) {
    digitalWrite(YMotorDirPin, YMotorDir);
    yStepper.runSpeed();
  }

  // Z Motor
  digitalWrite(ZMotorEnPin, ZMotorEn);
  if (ZMotorEn) {
    digitalWrite(ZMotorDirPin, ZMotorDir);
    zStepper.runSpeed();
  }

  // A Motor
  digitalWrite(AMotorEnPin, AMotorEn);
  if (AMotorEn) {
    digitalWrite(AMotorDirPin, AMotorDir);
    aStepper.runSpeed();
  }
}


#define USE_BASE
#define ARDUINO_ENC_COUNTER
#define L298_MOTOR_DRIVER
#undef USE_SERVOS

#define BAUDRATE     57600
#define MAX_PWM      255

#include "commands.h"
#include "sensors.h"

// No includes for .ino files—they get auto-merged by the IDE
#include "diff_controller.h"

#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

int arg=0, index=0;
char chr, cmd;
char argv1[16], argv2[16];
long arg1=0, arg2=0;

void resetCommand() {
  cmd = 0;
  memset(argv1,0,sizeof(argv1));
  memset(argv2,0,sizeof(argv2));
  arg = index = 0;
  arg1 = arg2 = 0;
}

int runCommand() {
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch(cmd) {
    case GET_BAUDRATE: Serial.println(BAUDRATE); break;
    case ANALOG_READ:  Serial.println(analogRead(arg1)); break;
    case DIGITAL_READ: Serial.println(digitalRead(arg1)); break;
    case ANALOG_WRITE: analogWrite(arg1,arg2); Serial.println("OK"); break;
    case DIGITAL_WRITE: digitalWrite(arg1,arg2?HIGH:LOW); Serial.println("OK"); break;
    case PIN_MODE:     pinMode(arg1,arg2?OUTPUT:INPUT); Serial.println("OK"); break;
    case PING:         Serial.println(Ping(arg1)); break;

#ifdef USE_BASE
    case READ_ENCODERS:
      Serial.print(readEncoder(LEFT)); Serial.print(" "); Serial.println(readEncoder(RIGHT));
      break;
    case RESET_ENCODERS:
      resetEncoders(); resetPID(); Serial.println("OK");
      break;
    case MOTOR_SPEEDS:
      lastMotorCommand = millis();
      if (arg1==0) {
        moving=0; resetPID(); setMotorSpeeds(0,0);
      } else {
        moving=1; pid.TargetTicksPerFrame=arg1;
      }
      Serial.println("OK");
      break;
#endif

    default: Serial.println("Invalid Command"); break;
  }
  return 0;
}

void setup() {
  Serial.begin(BAUDRATE);
#ifdef USE_BASE
  initMotorController();
  initEncoders();
  resetPID();
#endif
}

void loop() {
  while (Serial.available()>0) {
    chr = Serial.read();
    if (chr=='\r') {
      if(arg==1) argv1[index]='\0';
      runCommand();
      resetCommand();
    } else if (chr==' ') {
      if(arg==0) arg=1, index=0;
    } else {
      if(arg==0) cmd=chr;
      else if(arg==1) argv1[index++]=chr;
    }
  }

#ifdef USE_BASE
  static unsigned long nextPID=0;
  if (millis()>nextPID) {
    updatePID();
    nextPID=millis()+(1000/30);
  }
  if (millis()-lastMotorCommand > AUTO_STOP_INTERVAL) {
    moving=0; setMotorSpeeds(0,0);
  }
#endif
}

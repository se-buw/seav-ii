#ifndef DIFF_CONTROLLER_H
#define DIFF_CONTROLLER_H

#ifdef USE_BASE

// Forward declarations (Arduino IDE will merge .ino files)
long readEncoder(int i);
void setMotorSpeeds(int left, int right);

// PID state for the single controller (right‐wheel feedback)
typedef struct {
  double TargetTicksPerFrame;
  long   Encoder;
  long   PrevEnc;
  int    PrevInput;
  int    ITerm;
  long   output;
} PIDInfo;

static PIDInfo pid;
static unsigned char moving = 0;

// PID gains
static int Kp = 20;
static int Kd = 12;
static int Ki = 0;
static int Ko = 50;

// Reset the PID state
static void resetPID() {
  pid.TargetTicksPerFrame = 0;
  pid.Encoder             = readEncoder(RIGHT);
  pid.PrevEnc             = pid.Encoder;
  pid.PrevInput           = 0;
  pid.ITerm               = 0;
  pid.output              = 0;
}

// Called at ~30 Hz from loop()
static void updatePID() {
  pid.Encoder = readEncoder(RIGHT);
  if (!moving) {
    if (pid.PrevInput != 0) resetPID();
    return;
  }
  int input = pid.Encoder - pid.PrevEnc;
  long error = pid.TargetTicksPerFrame - input;
  long out = (Kp * error - Kd * (input - pid.PrevInput) + pid.ITerm) / Ko;
  pid.PrevEnc = pid.Encoder;
  out += pid.output;
  if      (out >  MAX_PWM) out =  MAX_PWM;
  else if (out < -MAX_PWM) out = -MAX_PWM;
  else                     pid.ITerm += Ki * error;
  pid.output    = out;
  pid.PrevInput = input;
  setMotorSpeeds(out, out);
}

#endif // USE_BASE
#endif // DIFF_CONTROLLER_H

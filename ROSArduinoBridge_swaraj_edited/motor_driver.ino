#ifdef USE_BASE

// SE ZK-BM1 (L298-style) pin definitions
#define RIGHT_MOTOR_FORWARD   5
#define RIGHT_MOTOR_BACKWARD  6
#define LEFT_MOTOR_FORWARD    9
#define LEFT_MOTOR_BACKWARD   10

void initMotorController() {
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  analogWrite(RIGHT_MOTOR_FORWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
  analogWrite(LEFT_MOTOR_FORWARD, 0);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
}

void setMotorSpeed(int i, int spd) {
  bool rev = false;
  if (spd < 0) { rev = true; spd = -spd; }
  if (spd > MAX_PWM) spd = MAX_PWM;

  if (i == LEFT) {
    analogWrite(LEFT_MOTOR_FORWARD, rev ? 0 : spd);
    analogWrite(LEFT_MOTOR_BACKWARD, rev ? spd : 0);
  } else {
    analogWrite(RIGHT_MOTOR_FORWARD, rev ? 0 : spd);
    analogWrite(RIGHT_MOTOR_BACKWARD, rev ? spd : 0);
  }
}

void setMotorSpeeds(int l, int r) {
  setMotorSpeed(LEFT, l);
  setMotorSpeed(RIGHT, r);
}

#endif // USE_BASE

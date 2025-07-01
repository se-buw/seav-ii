#ifdef USE_BASE

const byte hallPin = 3;
volatile unsigned long pulseCount = 0;

void onHallPulse() {
  pulseCount++;
}

void initEncoders() {
  pinMode(hallPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hallPin), onHallPulse, FALLING);
}

long readEncoder(int i) {
  if (i == RIGHT) {
    noInterrupts();
    long c = pulseCount;
    interrupts();
    return c;
  }
  return 0;
}

void resetEncoder(int i) {
  if (i == RIGHT) {
    noInterrupts();
    pulseCount = 0;
    interrupts();
  }
}

void resetEncoders() {
  resetEncoder(RIGHT);
}

#endif // USE_BASE

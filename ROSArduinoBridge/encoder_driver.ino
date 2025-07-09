/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table
    
  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR (PCINT2_vect){
  	static uint8_t enc_last=0;
        
	enc_last <<=2; //shift previous state two places
	enc_last |= (PIND & (3 << 2)) >> 2; //read the current state into lowest 2 bits
  
  	left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR (PCINT1_vect){
        static uint8_t enc_last=0;
          	
	enc_last <<=2; //shift previous state two places
	enc_last |= (PINC & (3 << 4)) >> 4; //read the current state into lowest 2 bits
  
  	right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }

// === HALL SENSOR ODOMETRY ===
#elif defined(ARDUINO_HALL_COUNTER)
  volatile long drive_ticks = 0L;
  volatile bool last_hall_state = LOW;
  volatile int last_motor_direction = 0;

  void updateDriveDirection(int dir) {
    last_motor_direction = dir;  // +10 = forward, -10 = reverse
  }

  void hallInterruptHandler() {
    bool hall_state = (PIND & (1 << HALL_ODOM_PIN)); // read pin PD2 directly
    if (hall_state && !last_hall_state) {
      drive_ticks += last_motor_direction;
    }
    last_hall_state = hall_state;
  }

  void initEncoder() {
    DDRD &= ~(1 << HALL_ODOM_PIN); // Set PD3 as input
    PORTD |= (1 << HALL_ODOM_PIN); // Enable pullup
    attachInterrupt(0, hallInterruptHandler, CHANGE); // Interrupt 0 = pin 3 = PD3
    drive_ticks = 0;
  }

  long readEncoder(int i) {
    if (i == 0) return drive_ticks;
    else return 0;
  }

  void resetEncoder(int i) {
    if (i == 0) drive_ticks = 0L;
  }

#elif defined(ARDUINO_HC89_COUNTER)
  volatile long drive_ticks = 0;
  volatile bool last_hc_state = LOW;
  volatile int  last_motor_direction = 0;

  void updateDriveDirection(int dir) { last_motor_direction = dir; }

  void hc89ISR() {
    bool s = (PIND & (1 << HC89_PIN));
    if (s && !last_hc_state) drive_ticks += last_motor_direction;
    last_hc_state = s;
  }

  void initEncoder() {
    DDRD  &= ~(1 << HC89_PIN);
    PORTD |=  (1 << HC89_PIN);           // pull-up
    attachInterrupt(0, hc89ISR, CHANGE); // INT0 = D2
    drive_ticks = 0;
  }

  long readEncoder(int i) {
    if (i == 0) return drive_ticks;
    else return 0;
  }

  void resetEncoder(int i) {
    if (i == 0) drive_ticks = 0L;
  }

#endif

// === ROTARY ENCODER STEERING ===

#ifdef ARDUINO_ROTARY_STATES
  volatile long rotary_ticks = 0L;
  volatile bool last_clk_state = LOW;

  void rotaryInterruptHandler() {
    bool clk_state = (PIND & (1 << CLOCK_ROTARY_A));  // PD7
    if (clk_state && !last_clk_state) {  // rising edge
      bool data_state = (PINB & (1 << DATA_ROTARY_B));  // PB0
      if (data_state != clk_state) {
        rotary_ticks++;  // clockwise
      } else {
        rotary_ticks--;  // counter-clockwise
      }
    }
    last_clk_state = clk_state;
  }

  void initRotary() {
    // Set pins as input with pullup
    DDRD &= ~(1 << CLOCK_ROTARY_A);
    PORTD |= (1 << CLOCK_ROTARY_A);
    DDRB &= ~(1 << DATA_ROTARY_B);
    PORTB |= (1 << DATA_ROTARY_B);

    // Enable pin change interrupt for PD7 (PCINT23)
    PCICR |= (1 << PCIE2);       // Enable pin-change interrupts for PORTD
    PCMSK2 |= (1 << CLOCK_ROTARY_A); // Enable interrupt on PD7 (PCINT23)

    last_clk_state = (PIND & (1 << CLOCK_ROTARY_A));
    rotary_ticks = 0;
  }

  ISR(PCINT2_vect) {
    rotaryInterruptHandler();
  }

  long readRotary(int i) {
    if (i == 1) return rotary_ticks * 23; // scale factor for degrees
    else return 0;
  }

  void resetRotary(int i) {
    if (i == 1) rotary_ticks = 0L;
  }
#endif


#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(0); // or LEFT
#ifdef ARDUINO_ROTARY_STATES
  resetRotary(1);  // or STEER
#endif
}


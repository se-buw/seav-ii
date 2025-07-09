/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ARDUINO_ENC_COUNTER
  //below can be changed, but should be PORTD pins; 
  //otherwise additional changes in the code are required
  #define LEFT_ENC_PIN_A PD2  //pin 2
  #define LEFT_ENC_PIN_B PD3  //pin 3
  
  //below can be changed, but should be PORTC pins
  #define RIGHT_ENC_PIN_A PC4  //pin A4
  #define RIGHT_ENC_PIN_B PC5   //pin A5
#endif

#ifdef ARDUINO_HALL_COUNTER
  #define HALL_ODOM_PIN   PD3   // digital pin 3 (for Hall odometry)
#endif

#ifdef ARDUINO_HC89_COUNTER           // NEW flag
  #define HC89_PIN        PD2         // D2 / INT0
#endif

#ifdef ARDUINO_ROTARY_STATES
  #define CLOCK_ROTARY_A  PD7   // digital pin 7 (steering encoder A)
  #define DATA_ROTARY_B   PB0   // digital pin 8 (steering encoder B)
#endif

long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
void initEncoder();
void initRotary();
long readRotary(int i);
void resetRotary(int i);
void updateDriveDirection(int dir);

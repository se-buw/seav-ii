/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  #define RIGHT_MOTOR_BACKWARD 5
  #define LEFT_MOTOR_BACKWARD  6
  #define RIGHT_MOTOR_FORWARD  9
  #define LEFT_MOTOR_FORWARD   10
  #define RIGHT_MOTOR_ENABLE 12
  #define LEFT_MOTOR_ENABLE 13
#endif

#ifdef ZKBM1_MOTOR_DRIVER
  #define DRIVE_PWM_IN1 5
  #define DRIVE_PWM_IN2 6 
  #define STEER_PWM_IN3 11
  #define STEER_PWM_IN4 12
  #define STEER_PWM_LEFT   180  // Adjust as needed for extreme left
  #define STEER_PWM_CENTER 128  // Neutral/stop PWM value
  #define STEER_PWM_RIGHT  180  // Adjust as needed for extreme right
#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
//void setMotorSpeeds(int leftSpeed, int rightSpeed);
void setMotorSpeeds(int driveSpeed, int steeringSpeed);
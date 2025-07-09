/***************************************************************
   Motor driver definitions
   
   Add a "#elif defined" block to this file to include support
   for a particular motor driver.  Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   *************************************************************/

#ifdef USE_BASE
   
#ifdef POLOLU_VNH5019
  /* Include the Pololu library */
  #include "DualVNH5019MotorShield.h"

  /* Create the motor driver object */
  DualVNH5019MotorShield drive;
  
  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined POLOLU_MC33926
  /* Include the Pololu library */
  #include "DualMC33926MotorShield.h"

  /* Create the motor driver object */
  DualMC33926MotorShield drive;
  
  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined L298_MOTOR_DRIVER
  void initMotorController() {
    digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);
    digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  }
  
  void setMotorSpeed(int i, int spd) {
    unsigned char reverse = 0;
  
    if (spd < 0)
    {
      spd = -spd;
      reverse = 1;
    }
    if (spd > 255)
      spd = 255;
    
    if (i == LEFT) { 
      if      (reverse == 0) { analogWrite(LEFT_MOTOR_FORWARD, spd); analogWrite(LEFT_MOTOR_BACKWARD, 0); }
      else if (reverse == 1) { analogWrite(LEFT_MOTOR_BACKWARD, spd); analogWrite(LEFT_MOTOR_FORWARD, 0); }
    }
    else /*if (i == RIGHT) //no need for condition*/ {
      if      (reverse == 0) { analogWrite(RIGHT_MOTOR_FORWARD, spd); analogWrite(RIGHT_MOTOR_BACKWARD, 0); }
      else if (reverse == 1) { analogWrite(RIGHT_MOTOR_BACKWARD, spd); analogWrite(RIGHT_MOTOR_FORWARD, 0); }
    }
  }
  
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }

#elif defined ZKBM1_MOTOR_DRIVER

  void initMotorController() {
    pinMode(DRIVE_PWM_IN1, OUTPUT);
    pinMode(DRIVE_PWM_IN2, OUTPUT);
    pinMode(STEER_PWM_IN3, OUTPUT);
    pinMode(STEER_PWM_IN4, OUTPUT);
    setMotorSpeeds(0, 0); // Stop all motors on init
  }

  // i: 0 = drive, 1 = steering
  void setMotorSpeed(int i, int spd) {
    spd = constrain(spd, -255, 255);

    if (i == DRIVE) {
      if (spd > 0) {
        analogWrite(DRIVE_PWM_IN1, spd);
        analogWrite(DRIVE_PWM_IN2, 0);
        updateDriveDirection(+1);
      } else if (spd < 0) {
        analogWrite(DRIVE_PWM_IN1, 0); 
        analogWrite(DRIVE_PWM_IN2, -spd);
        updateDriveDirection(-1);
      } else {
        digitalWrite(DRIVE_PWM_IN1, LOW);
        digitalWrite(DRIVE_PWM_IN2, LOW);
        updateDriveDirection(0);
      }
    }

    else if (i == STEER) {
      if (spd > 0) {
        analogWrite(STEER_PWM_IN3, spd);
        analogWrite(STEER_PWM_IN4, 0);
      } else if (spd < 0) {
        analogWrite(STEER_PWM_IN3, 0);
        analogWrite(STEER_PWM_IN4, -spd);
      } else {
        digitalWrite(STEER_PWM_IN3, LOW);
        digitalWrite(STEER_PWM_IN4, LOW);
      }
    }
  }

  // A convenience function for setting both channels
  void setMotorSpeeds(int driveSpeed, int steeringSpeed) {
    setMotorSpeed(DRIVE, driveSpeed);
    setMotorSpeed(STEER, steeringSpeed);
  }

  void setSteeringDirection(int dir) {
    switch(dir) {
        case -1:  // Extreme Left
            setMotorSpeed(STEER, -STEER_PWM_LEFT);  // Negative PWM for left
            break;
        case 0:   // Center
            setMotorSpeed(STEER, 0);  // Stop steering motor (hold center)
            break;
        case 1:   // Extreme Right
            setMotorSpeed(STEER, STEER_PWM_RIGHT);  // Positive PWM for right
            break;
        default:
            setMotorSpeed(STEER, 0);  // Default to center if invalid input
            break;
    }
  }

#else
  #error A motor driver must be selected!
#endif

#endif

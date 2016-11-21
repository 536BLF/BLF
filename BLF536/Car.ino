/*
 * Function: Car
 * Author: Alper Ender
 * Description: Constructor for the Car class
 */
Car::Car(){

}


/*
 * Function: Sample_Time
 * Author: Alper Ender
 * Description: Holds the processor here until the sampling time, T has been reached.
 *              Ensures that each loop through main takes approximately T seconds every time.
 */
void Car::Sample_Time(void){

  // Initializing the timer at the beginning
  if(this->timer == 0){ 
    this->timer = millis() + SAMPLING_TIME;
    return;
  }

  // Holds the processor here until the sampling time has been reached
  while(millis() < this->timer) {}

  // Updates the timer to the next sample time
  this->timer = this->timer + SAMPLING_TIME;
  
}


/*
 * Function: Read_Sensors_And_Obtain_Errors
 * Author: Alper Ender
 * Description: Reads both sensors, calculates the line value errors, and obtains the sensor error calc for 2 lines.
 *              NOTE: This should be changed when we switch to FSM design
 */
void Car::Read_Sensors_And_Obtain_Errors(void){

  // Reading all the sensors
  leftSensor.Read_All_Sensors();
  rightSensor.Read_All_Sensors();

  /*
  if(Check_Lost_Right_Line){
    rightSensor.Hold_Value();
  }

  if(Check_Lost_Left_Line){
    leftSensor.Hold_Value();
  }
  */

  // Calculating the line errors for each sensor
  leftSensor.Sensor_Calc();
  rightSensor.Sensor_Calc();

  // Calculating the position of the vehicle in respect to the set point
  this->pos = (leftSensor.sensedVal + rightSensor.sensedVal) / 2;

  // Calculating the total error for the vehicle
  this->totalError = this->pos - this->setPoint;
}


/*
 * Function: Controller
 * Author: Alper Ender
 * Description: Controller for the vehicle
 * 
 * NOTE:
 * 
 *      this->totalError         : Error of T = 0
 *      this->totalErrorHold[0]  : Error of T = -1
 *      this->totalErrorHold[1]  : Error of T = -2
 *      this->totalErrorHold[2]  : Error of T = -3
 *      this->totalErrorHold[3]  : Error of T = -4
 *
 *      this->motorDiffPWM          : Motor differential we are solving for (T = 0)
 *      this->motorDiffPWMHold[0]   : Motor Diff of T = -1
 *      this->motorDiffPWMHold[1]   : Motor Diff of T = -2
 *      this->motorDiffPWMHold[2]   : Motor Diff of T = -3
 *      this->motorDiffPWMHold[3]   : Motor Diff of T = -4
 * 
 */
void Car::Controller(){
  /*
 
  // Proportional controller, error times a constant = motor differential
  
  this->motorDiffPWM = KPROP * this->totalError; 
  
  */


  /*
  // ------ PID CONTROLLERS -------
  // ALL PID CONTROLLERS HAVE BEEN TESTED

  // INITIAL CONTROLLER
  // PID  Controller - SS: 160, K: 23
  // Results: STABLE - Postive responses with a little bit of overshoot
 
  #define PID_VAL_A             (3.7832L)
  #define PID_VAL_B             (-7.325L)
  #define PID_VAL_C             (3.543L)
  #define PID_VAL_D             (0.04089L)
  #define PID_VAL_E             (-0.07179L)
  #define PID_VAL_F             (0.03089L)
 
  this->motorDiffPWM = (PID_VAL_A * this->totalError + PID_VAL_B * this->totalErrorHold[0]   + PID_VAL_C * this->totalErrorHold[1] 
                                                     - PID_VAL_E * this->motorDiffPWMHold[0] - PID_VAL_F * this->motorDiffPWMHold[1]) / PID_VAL_D;
  */
  
  /*
  // PID  Controller - SS: 160, K: 23
  // Results: Unstable - Does not follow the line

  //       6.744 z^2 - 13.06 z + 6.321
  //  ----------------------------------
  //  0.01366 z^2 - 0.01732 z + 0.003659
 
  #define PID_VAL_A             (6.744L)
  #define PID_VAL_B             (-13.06L)
  #define PID_VAL_C             (6.321L)
  #define PID_VAL_D             (0.01366L)
  #define PID_VAL_E             (-0.01732L)
  #define PID_VAL_F             (0.003659L)
 
  this->motorDiffPWM = (PID_VAL_A * this->totalError + PID_VAL_B * this->totalErrorHold[0]   + PID_VAL_C * this->totalErrorHold[1] 
                                                     - PID_VAL_E * this->motorDiffPWMHold[0] - PID_VAL_F * this->motorDiffPWMHold[1]) / PID_VAL_D;
  */  

  /*
  // PID  Controller - SS: 140, K: 15
  // Results: Stable - But still oscillatory

  //       2.127 z^2 - 3.957 z + 1.841
  //  ----------------------------------
  //  0.01406 z^2 - 0.01812 z + 0.004061
 
  #define PID_VAL_A             (2.127L)
  #define PID_VAL_B             (-3.957L)
  #define PID_VAL_C             (1.841L)
  #define PID_VAL_D             (0.01406L)
  #define PID_VAL_E             (-0.01812L)
  #define PID_VAL_F             (0.004061L)
 
  this->motorDiffPWM = (PID_VAL_A * this->totalError + PID_VAL_B * this->totalErrorHold[0]   + PID_VAL_C * this->totalErrorHold[1] 
                                                     - PID_VAL_E * this->motorDiffPWMHold[0] - PID_VAL_F * this->motorDiffPWMHold[1]) / PID_VAL_D;
  */

  /*
  // PID  Controller - SS: 120, K: 10
  // Results: Unstable - Wheels change speed too fast and the vehicle goes off course

  //       -33.96 z^2 + 62.84 z - 29.06
  //  ----------------------------------
  //  0.01726 z^2 - 0.02451 z + 0.007256
 
  #define PID_VAL_A             (-33.96L)
  #define PID_VAL_B             (62.84L)
  #define PID_VAL_C             (-29.06L)
  #define PID_VAL_D             (0.01726L)
  #define PID_VAL_E             (-0.02451L)
  #define PID_VAL_F             (0.007256L)
 
  this->motorDiffPWM = (PID_VAL_A * this->totalError + PID_VAL_B * this->totalErrorHold[0]   + PID_VAL_C * this->totalErrorHold[1] 
                                                     - PID_VAL_E * this->motorDiffPWMHold[0] - PID_VAL_F * this->motorDiffPWMHold[1]) / PID_VAL_D;
  */

  /*
  // PID  Controller - SS: 100, K: 7
  // Results: Unstable - changes wheels too fast - does not follow the line

  //       35.67 z^2 - 69.63 z + 33.98
  //  ----------------------------------
  //  0.01525 z^2 - 0.02049 z + 0.005246
 
  #define PID_VAL_A             (35.67L)
  #define PID_VAL_B             (-69.63L)
  #define PID_VAL_C             (33.98L)
  #define PID_VAL_D             (0.01525L)
  #define PID_VAL_E             (-0.02049L)
  #define PID_VAL_F             (0.005246L)
 
  this->motorDiffPWM = (PID_VAL_A * this->totalError + PID_VAL_B * this->totalErrorHold[0]   + PID_VAL_C * this->totalErrorHold[1] 
                                                     - PID_VAL_E * this->motorDiffPWMHold[0] - PID_VAL_F * this->motorDiffPWMHold[1]) / PID_VAL_D;
  */


  /*
  // Alper PID TEST
  // Results: Unstable - Does not follow the line

  //   434.9 z^2 - 849.9 z + 415.2
  //  ---------------------------
  //   z^2 - 1.333 z + 0.3325

  #define A             (415.2L)
  #define B             (-849.9L)
  #define C             (434.9L)
  #define D             (-1.333L)
  #define E             (0.3325L)
 
  double tmpA = A * this->totalError;
  double tmpB = B * this->totalErrorHold[0];
  double tmpC = C * this->totalErrorHold[1];
  double tmpD = D * this->motorDiffPWMHold[0];
  double tmpE = E * this->motorDiffPWMHold[1];

  this->motorDiffPWM = tmpA + tmpB + tmpC - tmpD - tmpE;
  */
  
  
  /*
  // ------ SISO LAG/LEAD CONTROLLERS -------
  // ALL LEAD LAG CONTROLLERS HAVE BEEN TESTED

  // SISO Lead/Lag Controller - SS: 160, K:23
  // Results: Unstable - oscillates too much

  #define A            (25.52L)
  #define B            (-10.21L)
  #define C            (-0.043L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];
  */

  /*
  // SISO Lead/Lag Controller - SS: 160, K:23
  // Results: Unstable - Does not follow the line at all

  // (1.826 z - 0.1771)/(z-0.03)

  #define A            (1.826L)
  #define B            (-0.1771L)
  #define C            (-0.03L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];
  */

  /*
  // SISO Lead/Lag Controller - SS: 160, K:23
  // Results: Unstable - Does not follow the line at all

  // (0.9839z-0.09741)/(z-0.015)

  #define A            (0.9839L)
  #define B            (-0.09741L)
  #define C            (-0.015L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];
  */

  /*
  // SISO Lead/Lag Controller - SS: 160, K:23
  // Results: Unstable - Oscillates way too much

  // (80.73z-0.7992)/(z-0.0009048)

  #define A            (80.73L)
  #define B            (-0.7992L)
  #define C            (-0.0009048L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];  
  */

  /*
  // SISO Lead/Lag Controller - SS: 160, K:23
  // Results: Unstable - Oscillates way too much. Wheels either are the same or extremely fast

  // (139.3z-39.86)/(z-0.07)

  #define A            (139.3L)
  #define B            (-39.86L)
  #define C            (-0.07L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];  
  */

  /*
  // SISO Lead/Lag Controller - SS: 140, K:15
  // Results: STABLE - One of the best controllers so far

  // (272.9z-259.2)/(z-0.07)

  #define A            (272.9L)
  #define B            (-259.2L)
  #define C            (-0.07L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];  
  */

  /*
  // SISO Lead/Lag Controller - SS: 100, K:7
  // Results: Stable but oscillatory - almost marginally unstable

  // (1000z-950)/(z-0.95)

  #define A            (1000L)
  #define B            (-950L)
  #define C            (-0.95L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];  
  */

  /*
  // SISO Lead/Lag Controller - SS: 100, K:7
  // Results: Stable - A little Jerky

  // (1239z-1199)/(z-0.009)

  #define A            (1239L)
  #define B            (-1199L)
  #define C            (-0.009L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] - C * this->motorDiffPWMHold[0];  
  */
  

  /*
  // ------ ROOT LOCUS CONTROLLERS -------
  // ALL ROOT LOCUS CONTROLLERS HAVE BEEN TESTED

  // Root Locus Controller - SS: 160, K:23
  // Results: Unstable - Does not follow the line

  //  29.5 z^2 - 59.59 z + 30.12
  //  --------------------------
  //      z^2 - 1.5 z + 0.5

  #define A        (29.5L)
  #define B        (-59.59L)
  #define C        (30.12L)
  #define D        (-1.5L)
  #define E        (0.5L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] + C * this->totalErrorHold[1] - D * this->motorDiffPWMHold[0] - E * this->motorDiffPWMHold[1];
  */

  /*
  // Root Locus Controller - SS: 160, K:23 part 2
  // Results: Unstable - Does not follow the line

  //  4190 z^2 - 8393 z + 4215
  //  ------------------------
  //    z^2 - 1.25 z + 0.25

  #define A        (4190L)
  #define B        (-8393L)
  #define C        (4215L)
  #define D        (-1.25L)
  #define E        (0.25L)

  double tmpA = A * this->totalError;
  double tmpB = B * this->totalErrorHold[0];
  double tmpC = C * this->totalErrorHold[1];
  double tmpD = D * this->motorDiffPWMHold[0];
  double tmpE = E * this->motorDiffPWMHold[1];

  this->motorDiffPWM = tmpA + tmpB + tmpC - tmpD - tmpE;
  */
  

  /*
  // Root Locus Controller - SS: 140, K:15
  // Results: Unstable - Does not follow the line

  //  92.2 z^2 - 187.5 z + 95.7
  //  -------------------------
  //      z^2 - 1.1 z + 0.1

  #define A        (92.2L)
  #define B        (-187.5L)
  #define C        (95.7L)
  #define D        (-1.1L)
  #define E        (0.1L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] + C * this->totalErrorHold[1] - D * this->motorDiffPWMHold[0] - E * this->motorDiffPWMHold[1];
  */

  /*
  // Root Locus Controller - SS: 120, K:10
  // Results: Unstable - Very slow response time

  //  250 z^2 - 499.5 z + 250
  //  -----------------------
  //          z^2 - z

  #define A        (250L)
  #define B        (-499.5L)
  #define C        (250L)
  #define D        (-1L)
  #define E        (0L)

  double tmpA = A * this->totalError;
  double tmpB = B * this->totalErrorHold[0];
  double tmpC = C * this->totalErrorHold[1];
  double tmpD = D * this->motorDiffPWMHold[0];
  double tmpE = E * this->motorDiffPWMHold[1];

  this->motorDiffPWM = tmpA + tmpB + tmpC - tmpD - tmpE;
  */

  /*
  // Root Locus Controller - SS: 100, K:7
  // Results: Unstable - very oscillatory

  //  58000 z^2 - 1.159e05 z + 5.8e04
  //  -------------------------------
  //           z^2 - 2 z + 1

  #define A        (58000L)
  #define B        (-0.00001159L)
  #define C        (58000L)
  #define D        (-2L)
  #define E        (1L)

  double tmpA = A * this->totalError;
  double tmpB = B * this->totalErrorHold[0];
  double tmpC = C * this->totalErrorHold[1];
  double tmpD = D * this->motorDiffPWMHold[0];
  double tmpE = E * this->motorDiffPWMHold[1];

  this->motorDiffPWM = tmpA + tmpB + tmpC - tmpD - tmpE;
  */



  /*
  // ------ PREDICTION OBSERVER CONTROLLERS -------
  // Prediction Observer Controller - SS: 160
  // Results: Unstable - Does not follow the line

  //                    2.457 z - 2.599
  //  D(z) = ----------------------------- 
  //             z^2 - 1.838 z + 0.8499

  #define A        (2.457L)
  #define B        (-2.599L)
  #define C        (-1.838L)
  #define D        (0.8499L)

  this->motorDiffPWM = A * this->totalErrorHold[0] + B * this->totalErrorHold[1] - C * this->motorDiffPWMHold[0] - D * this->motorDiffPWMHold[1];
  */

  /*
  // Prediction Observer Controller - SS: 160
  // Results: Stable - One of the best so far. A tad jerky

  //              578.1 z - 567.1
  // Dz = ------------------------------
  //         z^2 - 0.1097 z - 0.1143

  #define A        (578.1L)
  #define B        (-567.1L)
  #define C        (-0.1097L)
  #define D        (-0.1143L)

  this->motorDiffPWM = A * this->totalErrorHold[0] + B * this->totalErrorHold[1] - C * this->motorDiffPWMHold[0] - D * this->motorDiffPWMHold[1];
  */



  /*

  // ------ CURRENT OBSERVER CONTROLLERS -------

  */



  /*
  // ------ IH-LQR CONTROLLERS -------
  // IH-LQR Controller - SS: 160
  // Results: Stable - A bit jerky

  //      2587 z^2 - 1930 z
  //  -------------------------
  //  z^2 - 0.4313 z + 0.003083

  #define A        (2587L)
  #define B        (-1930L)
  #define C        (0L)
  #define D        (-.4313L)
  #define E        (0.003083L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] + C * this->totalErrorHold[1] - D * this->motorDiffPWMHold[0] - E * this->motorDiffPWMHold[1];
  */

  /*
  // IH-LQR Controller - SS: 140
  // Results: Marginally Stable - Very oscillatory

  //      417.3 z^2 - 36.24 z
  //  ---------------------------
  //  z^2 - 0.08635 z + 4.477e-07

  #define A        (417.3L)
  #define B        (-36.24L)
  #define C        (0L)
  #define D        (-0.08635L)
  #define E        (0.0000004477L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] + C * this->totalErrorHold[1] - D * this->motorDiffPWMHold[0] - E * this->motorDiffPWMHold[1];
  */

  /*
  // IH-LQR Controller - SS: 120
  // Results: Marginally Stable - Very oscillatory

  //      419.8 z^2 + 0.5545 z
  //  ----------------------------
  //  z^2 + 0.001321 z - 6.808e-11

  #define A        (419.8L)
  #define B        (0.5545L)
  #define C        (0L)
  #define D        (0.001321L)
  #define E        (-0.00000000006808L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] + C * this->totalErrorHold[1] - D * this->motorDiffPWMHold[0] - E * this->motorDiffPWMHold[1];
  */

  /*
  // IH-LQR Controller - SS: 100
  // Results: Marginally Stable - Very oscillatory

  //      419.1 z^2 - 10.47 z
  //  ---------------------------
  //  z^2 - 0.02494 z + 1.287e-07

  #define A        (419.1L)
  #define B        (-10.47L)
  #define C        (0L)
  #define D        (-0.02494L)
  #define E        (-0.0000001287L)

  this->motorDiffPWM = A * this->totalError + B * this->totalErrorHold[0] + C * this->totalErrorHold[1] - D * this->motorDiffPWMHold[0] - E * this->motorDiffPWMHold[1];
  */
  
}


/*
 * Function: MotorDiff
 * Author: Ben Wagner
 * Description: Motor Control, saves the current error and motor differential values
 */
void Car::MotorDiff(){
  double rightSpeed, leftSpeed;
  int left, right;
 
  // Assume diffvalue < 0 as RIGHT turn and > 0 is a LEFT turn
  if(this->motorDiffPWM > (MAX_SPEED_VAL - this->pwmSetSpeed)){
    this->motorDiffPWM = MAX_SPEED_VAL - this->pwmSetSpeed;
  }
  else if (this->motorDiffPWM < (this->pwmSetSpeed - MAX_SPEED_VAL)){
    this->motorDiffPWM = (this->pwmSetSpeed - MAX_SPEED_VAL);
  }

  // Pushes the current motor differential PWM value onto the hold array
  Push_Onto_Array(this->motorDiffPWMHold, this->motorDiffPWM, HOLD_AMOUNT);
  
  // Pushes the current total error value in the hold array
  Push_Onto_Array(this->totalErrorHold, this->totalError, HOLD_AMOUNT);

  // Setting the left speed and the right speed
  leftSpeed   = this->pwmSetSpeed + (this->motorDiffPWM);
  rightSpeed  = this->pwmSetSpeed - (this->motorDiffPWM);

  // Ensuring the values are between 0-255
  if(leftSpeed  > 255){ leftSpeed   = 255; }
  if(rightSpeed > 255){ rightSpeed  = 255; }
  if(leftSpeed  < 0)  { leftSpeed   = 0; }
  if(rightSpeed < 0)  { rightSpeed  = 0; }

  // Casting the speeds as integers for the motors
  left  = (int)leftSpeed;
  right = (int)rightSpeed;

  // Serial.print("\t,LEFT:\t"); Serial.print(left);
  // Serial.print("\t,RIGHT:\t"); Serial.print(right);
  // Serial.print("\t");

  // Throwing the motor values into the motors
  MotorRightPtr->setSpeed(right);
  MotorLeftPtr->setSpeed(left);
  MotorRightPtr->run(FORWARD);
  MotorLeftPtr->run(FORWARD);
}


/*
 * Function: Push_Onto_Array
 * Author: Alper Ender
 * Description: Pushes a value onto the top of an array to hold
 * 
 */
void Push_Onto_Array(double input[], double pushVal, int arraySize){
  int i;
  
  for(i=arraySize - 2 ; i >= 0 ; i--){
    input[i+1] = input[i];
  }
  
  input[0] = pushVal;
}


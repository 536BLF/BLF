/*
 * Function: Car
 * Author: Alper Ender
 * Description: Constructor for the Car class - Does not do anything.
 */
Car::Car(){

}


/*
 * Function: MotorDiff
 * Author: Ben Wagner
 * Description: Motor Control
 */
void Car::MotorDiff(){
  double rightSpeed;
  int left;
  double leftSpeed;
  int right;
 
  //assume diffvalue < 0 as right turn and >0 is a LEFT turn
  if(this->motorDiffPWM > (MAX_SPEED_VAL - this->pwmSetSpeed)){
    this->motorDiffPWM = MAX_SPEED_VAL - this->pwmSetSpeed;
  }
  else if (this->motorDiffPWM < (this->pwmSetSpeed - MAX_SPEED_VAL)){
    this->motorDiffPWM = (this->pwmSetSpeed - MAX_SPEED_VAL);
    }

  Push_Onto_Array(this->totalErrorHold, this->totalError, HOLD_AMOUNT);
  Push_Onto_Array(this->motorDiffPWMHold, this->motorDiffPWM, HOLD_AMOUNT);
    
  leftSpeed = this->pwmSetSpeed + (this->motorDiffPWM);
  rightSpeed = this->pwmSetSpeed - (this->motorDiffPWM);

  if(leftSpeed  > 255){ leftSpeed   = 255; }
  if(rightSpeed > 255){ rightSpeed  = 255; }
  if(leftSpeed  < 0)  { leftSpeed   = 0; }
  if(rightSpeed < 0)  { rightSpeed  = 0; }

  left = (int)leftSpeed;
  right = (int)rightSpeed;

  // Serial.print("\t,LEFT:\t"); Serial.print(left);
  // Serial.print("\t,RIGHT:\t"); Serial.print(right);
  // Serial.print("\t");

  MotorRightPtr->setSpeed(right);
  MotorLeftPtr->setSpeed(left);
  MotorRightPtr->run(FORWARD);
  MotorLeftPtr->run(FORWARD);
  

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

  // Calculating the line errors for each sensor
  leftSensor.Sensor_Calc();
  rightSensor.Sensor_Calc();

  // Calculating the position of the vehicle in respect to the set point
  this->pos = (leftSensor.sensedVal + rightSensor.sensedVal) / 2;

  // Calculating the total error for the vehicle
  this->totalError = this->pos - this->setPoint;
  
}


/*
 * Function: System_Identification
 * Author: Alper Ender
 * Description: The code for the system identification
 *              Description of the system identification:
 *                  1. Initialize the time to begin Sys Id
 *                  2. Continue straight until the begin time is hit
 *                  3. Create an impulse error of 1 cm for 1 time sample
 *                  4. Bring the error back down to zero
 */
void Car::System_Identification(void){

  // Initializing our beginning time
  if(this->beginTime == 0){
    this->beginTime = millis() + BEGIN_TIME;
    this->deltaTime = millis() + BEGIN_TIME + DELTA_TIME;
    return;
  }

  // Do not do anything if we have not hit the beginning time
  if(this->beginTime > millis()) return;

  // Return if we have already completed the impulse
  if(this->signalEnd) return;

  // If we have not began the impulse - create the impulse by changing the initialized line value
  if(!this->signalBegin){
    this->setPoint = 1;

    // Create this impulse once
    this->signalEnd = true;
    this->signalBegin = true;
  }
  
}


/*
 * Function: Controller
 * Author: Alper Ender
 * Description: Multiplies the error with the Kprop
 */
void Car::Controller(){
  /*
  // Proportional controller, error times a constant = motor differential
  this->motorDiffPWM = KPROP * this->totalError; 
  */

  this->motorDiffPWM = (PID_VAL_A * this->totalError + PID_VAL_B * this->totalErrorHold[0] + PID_VAL_C * this->totalErrorHold[1] 
                      - PID_VAL_E * this->motorDiffPWMHold[0] - PID_VAL_F * this->motorDiffPWMHold[1]) / PID_VAL_D;
  
}


/*
 * Function: Push_Onto_Array
 * Author: Alper Ender
 * Description: Pushes a value onto the top of an array
 */
void Push_Onto_Array(double input[], double pushVal, int arraySize){
  int i;
  
  for(i=arraySize - 2 ; i >= 0 ; i--){
    input[i+1] = input[i];
  }
  
  input[0] = pushVal;
  
}


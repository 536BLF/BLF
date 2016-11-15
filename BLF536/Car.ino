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
  double leftSpeed;
 
  //assume diffvalue < 0 as right turn and >0 is a LEFT turn
  if(this->motorDiffPWM > (MAX_SPEED_VAL - this->pwmSetSpeed)){
    this->motorDiffPWM = MAX_SPEED_VAL - this->pwmSetSpeed;
  }
  else if (this->motorDiffPWM < (this->pwmSetSpeed - MAX_SPEED_VAL)){
    this->motorDiffPWM = (this->pwmSetSpeed - MAX_SPEED_VAL);
    }
    
  leftSpeed = this->pwmSetSpeed + (this->motorDiffPWM);
  rightSpeed = this->pwmSetSpeed - (this->motorDiffPWM);

  if(leftSpeed  > 255){ leftSpeed   = 255; }
  if(rightSpeed > 255){ rightSpeed  = 255; }
  if(leftSpeed  < 0)  { leftSpeed   = 0; }
  if(rightSpeed < 0)  { rightSpeed  = 0; }

  //Serial.print("\t,LEFT:\t"); Serial.print(leftSpeed);
  //Serial.print("\t,RIGHT:\t"); Serial.print(rightSpeed);
  //Serial.print("\t");

  this->MotorRightPtr->setSpeed(rightSpeed);
  this->MotorLeftPtr->setSpeed(leftSpeed);
  this->MotorRightPtr->run(FORWARD);
  this->MotorLeftPtr->run(FORWARD);
 
}


/*
 * Function: Total_Error_Calc
 * Author: Alper Ender
 * Description: Obtains the total error calculation depending on which sensors are sensing the line
 */
void Car::Total_Error_Calc(int select){
  
  switch(select){
    
    // When the line for the left sensor has dissappeared - use the left sensor error
    case 0:         
      this->totalError = leftSensor.error; 
      break;      

    // When the line for the right sensor has dissappeared - use the right sensor error
    case 1:
      this->totalError = rightSensor.error;    
      break;      

    // When sensing both sensors over the line - average the errors
    case 2:
      /*
      if(rightSensor.error > 5 && leftSensor.error > 5){
        rightSensor.error = 0;
        leftSensor.error = 0;
      } else if(rightSensor.error > 5){
        rightSensor.error = leftSensor.error;
      } else if(leftSensor.error > 5){
        leftSensor.error = rightSensor.error;
      }
      */
      this->totalError = (leftSensor.error + rightSensor.error) / (float)2;
      break;   
  }
  if(this->totalError > 5) this->totalError = 0;
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
  leftSensor.Sensor_Error_Calc();
  rightSensor.Sensor_Error_Calc();

  // Calculating the total error for 2 lines
  this->Total_Error_Calc(2);
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
  if(this->impulseEnd) return;

  // If we have not began the impulse - create the impulse by changing the initialized line value
  if(!this->impulseBegin){
    rightSensor.initLineVal = rightSensor.initLineVal + IMPULSE_VALUE;
    leftSensor.initLineVal = leftSensor.initLineVal + IMPULSE_VALUE;
    this->impulseBegin = true;
  /*
  // End the impulse
  // In the next time sample after we began our impulse, we will be here to bring the initialized value back to 0, completing the impulse
  } else {
    rightSensor.initLineVal = rightSensor.initLineVal + IMPULSE_VALUE;
    leftSensor.initLineVal = leftSensor.initLineVal + IMPULSE_VALUE;
  */
    // Create this impulse once
    this->impulseEnd = true;
  }
  
}


void Car::Controller(){
  // Proportional controller, error times a constant = motor differential
  this->motorDiffPWM = KPROP * this->totalError; 
}


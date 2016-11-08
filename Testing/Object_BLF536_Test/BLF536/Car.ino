Car::Car()
{
  Kprop = KPROP;
  refPoint = 0;
  //AF_DCMotor motorRight(1, MOTOR12_64KHZ);
  //AF_DCMotor motorLeft(2, MOTOR12_64KHZ);
  //rightSensor.a;
}


void Car::MotorDiff()
{
  int RightSpeed;
  int LeftSpeed;
  
  //assume diffvalue>0 as right turn and <0 is a right turn
  LeftSpeed=this->pwmSetSpeed-(this->motorDiffPWM/2);
  RightSpeed=this->pwmSetSpeed+(this->motorDiffPWM/2);
  
  //motorRight.setSpeed(RightSpeed);
  //motorLeft.setSpeed(LeftSpeed);
  
  //motorRight.run(FORWARD);
  //delay (MOTORDELAY)
  //motorLeft.run(FORWARD);
 
}


void Car::Total_Error_Calc(int select){
  switch(select){
    case 0:
      this->totalError = leftSensor.error;                // When the line for sensor 1 has disappeared

    case 1:
      this->totalError = rightSensor.error;                // When the line for sensor 0 has disappeared

    case 2:
      this->totalError = ((-leftSensor.error) + rightSensor.error)/2;   // Average of the sensor errors
  }
}


void Car::sysId()
{
  if(millis()<ZEROREFTIME) refPoint = 0; //Reference point is zero until time specified, then goes to 1. (step function)
  else refPoint = 1;
  
  motorDiffPWM = Kprop*totalError; //Proportional controller, error times a constant = motor differential
}


void Car::Sample_Time(void){
  if(this->timer == 0){ 
    this->timer = millis() + SAMPLING_TIME;
    return;
  }
  
  while(millis() < this->timer) {}
  
  this->timer = this->timer + SAMPLING_TIME;
  
  
}


void Car::System_Identification(void){
  
  if(this->delta < millis()) return;
  
  if(this->sysIdCompleted) return;
  
  rightSensor.initLineVal = rightSensor.initLineVal - 1000UL;
  leftSensor.initLineVal = leftSensor.initLineVal - 1000UL;
  this->sysIdCompleted = true;
  
}

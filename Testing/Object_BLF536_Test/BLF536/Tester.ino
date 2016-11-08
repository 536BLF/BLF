Tester::Tester(){
  
}


void Tester::Print_Sensor_Values(void){
  int i;

  Serial.print("Pin "); Serial.print(leftSensor.pin); Serial.print(" Sensors: ");
  for (i = 0 ; i < NUM_SENSORS ; i++) {
    Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(leftSensor.sensorVals_T0[i]);
  }
  
  Serial.print("\tPin "); Serial.print(rightSensor.pin); Serial.print(" Sensors: ");
  for (i = 0 ; i < NUM_SENSORS ; i++) {
    Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(rightSensor.sensorVals_T0[i]);
  }
  
  Serial.println("");
}


void Tester::Print_Line_Values(void){
  leftSensor.Get_Line_Value();
  Serial.print("Left:\t"); Serial.print(leftSensor.lineVal); Serial.print("\t");
  rightSensor.Get_Line_Value();
  Serial.print("Right:\t"); Serial.println(rightSensor.lineVal);
}


void Tester::Print_Total_Error(void){
  Serial.print("LeftINIT:\t"); Serial.print(leftSensor.initLineVal); Serial.print("\t");
  Serial.print("RightINIT:\t"); Serial.print(rightSensor.initLineVal); Serial.print("\t");
  leftSensor.Sensor_Error_Calc();
  Serial.print("Left:\t"); Serial.print(leftSensor.error); Serial.print("\t");
  rightSensor.Sensor_Error_Calc();
  Serial.print("Right:\t"); Serial.print(rightSensor.error); Serial.print("\t");
  BLF536.Total_Error_Calc(2);
  Serial.print("Total:\t"); Serial.println(BLF536.totalError);
}


/*
 * Function: Start_Timer
 * Description: Starts a local timer
 */
void Tester::Start_Timer(void){
  this->timerVal = micros();
}

/*
 * Function: Display Timer
 * Description: Displays the amount of time in MICROSECONDS that has gone by since the timer was started
 */
void Tester::Display_Timer(void){
  Serial.println(micros() - this->timerVal);
}



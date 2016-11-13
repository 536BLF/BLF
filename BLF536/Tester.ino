/*
 * Function: Tester
 * Author: Alper Ender
 * Description: Constructor of the Tester class - This function does not do anything
 */
Tester::Tester(){
  
}


/*
 * Function: Print_Sensor_Values
 * Author: Alper Ender
 * Description: Prints the entire sensor array for both the left and right sensors
 */
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


/*
 * Function: Print_Line_Values
 * Author: Alper Ender
 * Description: Obtains the line values for the sensors and Prints the Line Values for each sensor
 */
void Tester::Print_Line_Values(void){
  leftSensor.Get_Line_Value();
  Serial.print("Left:\t"); Serial.print(leftSensor.lineVal); Serial.print("\t");
  rightSensor.Get_Line_Value();
  Serial.print("Right:\t"); Serial.println(rightSensor.lineVal);
}


/*
 * Function: Print_Total_Error
 * Author: Alper Ender
 * Description: Obtains the error valeus and prints the total error values for the car
 */
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
 * Author: Alper Ender
 * Description: Starts a local timer
 */
void Tester::Start_Timer(void){
  this->timerVal = micros();
}

/*
 * Function: Display Timer
 * Author: Alper Ender
 * Description: Displays the amount of time in MICROSECONDS that has gone by since the timer was started
 */
void Tester::Display_Timer(void){
  Serial.println(micros() - this->timerVal);
}


/*
 * Function: SystemId
 * Author: Alper Ender
 * Description: Prints the sample number and the total error for the system identification in a CSV format
 */
void Tester::SystemId(void){
  if(this->sample < MAX_NUM_SAMPLES && BLF536.beginTime < millis()){
    Serial.print(this->sample);
    Serial.print(",");
    Serial.print(BLF536.totalError);
    Serial.println(",");
    this->sample++;
  }
}


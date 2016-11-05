
/*
 * Function: FSM
 * Description: The finite state machine to determine which state the vehicle is currently in
 * 
 * --- THIS FUNCTION IS CURRENTLY INCOMPLETE - 11/5 ---
 * 
 */
void FSM(void){
  switch(state){
    case INIT: break;
    case READ_2_LINE: break;
    case READ_ANALOG0_LINE: break;
    case READ_ANALOG1_LINE: break;
    case READ_NO_LINE: break;
    case INTERSECTION: break;
    default: state = INIT;
  }
}


/*
 * Function: Check_Intersection
 * Description: Checks the RIGHT sensor to see if it sees a corner. If it does, then it changes the FSM to the intersection state to handle this intersection
 */
void Check_Intersection(void){
  if(analog1[4] > ON_LINE_THRESHOLD && analog1[5] > ON_LINE_THRESHOLD && analog1[6] > ON_LINE_THRESHOLD && analog1[7] > ON_LINE_THRESHOLD) 
    state = INTERSECTION;
}


/*
 * Function: Check_Lost_Analog1_Line
 * Description: checks to see if the analog1 sensor lost the line. Returns 1 if it did, else it returns 0
 */
int Check_Lost_Analog1_Line(void){
  if(analog1[0] < OFF_LINE_THRESHOLD && analog1[1] < OFF_LINE_THRESHOLD && analog1[2] < OFF_LINE_THRESHOLD && analog1[3] < OFF_LINE_THRESHOLD 
  && analog1[4] < OFF_LINE_THRESHOLD && analog1[5] < OFF_LINE_THRESHOLD && analog1[6] < OFF_LINE_THRESHOLD && analog1[7] < OFF_LINE_THRESHOLD)
    return 1;

  return 0;
}


/*
 * Function: Check_Lost_Analog0_Line
 * Description: checks to see if the analog0 sensor lost the line. Returns 1 if it did, else it returns 0
 */
int Check_Lost_Analog0_Line(void){
  if(analog0[0] < OFF_LINE_THRESHOLD && analog0[1] < OFF_LINE_THRESHOLD && analog0[2] < OFF_LINE_THRESHOLD && analog0[3] < OFF_LINE_THRESHOLD 
  && analog0[4] < OFF_LINE_THRESHOLD && analog0[5] < OFF_LINE_THRESHOLD && analog0[6] < OFF_LINE_THRESHOLD && analog0[7] < OFF_LINE_THRESHOLD)
    return 1;

  return 0;
}


/*
 * Function: Array_Copy
 * Description: Copies the sensor array into an older array to keep the old value of the sensors.
 */
void Array_Copy(int select){
  int i;
  switch(select){
    case PIN_ANALOG_0:

      for(i=0 ; i<NUM_SENSORS ; i++){
        oldAnalog0[i] = analog0[i];
      }
      break;

    case PIN_ANALOG_1:

      for(i=0 ; i<NUM_SENSORS ; i++){
        oldAnalog1[i] = analog1[i];
      }
      break;
    
  }
}


/*
 * Function: Read_2_Line
 * Description: The function that handles the READ_2_LINES state of the FSM. Reads 2 solid lines.
 * 
 * --- THIS FUNCTION IS CURRENTLY INCOMPLETE - 11/5 ---
 * 
 */
void Read_2_Line(void){
  
}


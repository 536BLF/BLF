/*----------------------------------------------------------------------
 *
 *    THESE FUNCTIONS ARE NOT COMPLETED NOR ARE THEY TESTED  (11/25)   
 *
 * --------------------------------------------------------------------*/

/**************************************************************
 * 
 * Function: FSM
 * Author: Ben Wagner and Alper Ender
 * Description: The finite state machine to determine the state of the vehicle
 * 
 * --- THIS FUNCTION IS CURRENTLY INCOMPLETE - 11/5 ---
 * 
 *************************************************************/
void FSM(void) {
  Determine_State();
  
  switch (state) {
    
    case INIT: break;
    
    case READ_2_LINE:     {
        Follow_2_Line();
        break;
      }
      
    case READ_LEFT_LINE:  {
        Follow_Left();
        break;
      }
      
    case READ_RIGHT_LINE: {
        Follow_Right();
        break;
      }

    case READ_NO_LINE:    {
        Open_Run();
        break;
      }
      
    case INTERSECTION:    {
        Intersection();
        break;
      }
      
    default: state = INIT;
    
  }
}


/**************************************************************
 * 
 * Function: Determine_State
 * Author: Alper Ender and Ben Wagner
 * Description: The function that determines the next state based on what the sensors are seeing.
 *
 * --- THIS FUNCTION IS CURRENTLY INCOMPLETE - 11/8 ---
 *
 **************************************************************/
void Determine_State(void) {
  boolean lostLeft, lostRight;

  // Checking lost left or right lines
  lostLeft  = Check_Lost_Left_Line();
  lostRight = Check_Lost_Right_Line();

  // Lost Both Lines
  if(lostLeft && lostRight) {
    state = READ_RIGHT_LINE;
    // state = READ_NO_LINE;
  }

  // Lost Left Line
  if(lostLeft && !lostRight) {
    state = READ_RIGHT_LINE;
  }

  // Lost Right Line
  if(!lostLeft && lostRight) {
    state = READ_RIGHT_LINE;
    // state = READ_LEFT_LINE;
  }

  // Lost No Lines
  if(!lostLeft && !lostRight) {
    state = READ_RIGHT_LINE;
    // state = READ_2_LINE;
  }

  // Checking for Intersection
  if(Check_Intersection() > 7000) {
    state = INTERSECTION;
  }
}


/**************************************************************
 * 
 * Function: Check_Intersection
 * Author: Alper Ender
 * Description: Checks the RIGHT sensor to see if it sees a corner. If it does, then it changes the FSM to the intersection state to handle this intersection
 * 
 **************************************************************/
double Check_Intersection(void) {
  double sum = 0;

  for(int i = 0 ; i < 7 ; i++){
    sum += rightSensor.sensorVals_T0[i];
  }
  // Serial.println(sum);

  return sum;
}


/**************************************************************
 * 
 * Function: Check_Lost_Right_Line
 * Author: Alper Ender
 * Description: Checks to see if the right sensor lost the line. Returns 1 if it did, else it returns 0
 * 
 **************************************************************/
boolean Check_Lost_Right_Line(void) {
  if (       rightSensor.sensorVals_T0[0] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[1] < OFF_LINE_THRESHOLD
         &&  rightSensor.sensorVals_T0[2] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[3] < OFF_LINE_THRESHOLD
         &&  rightSensor.sensorVals_T0[4] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[5] < OFF_LINE_THRESHOLD
         &&  rightSensor.sensorVals_T0[6] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[7] < OFF_LINE_THRESHOLD) {
    return true;
  }

  return false;
}


/**************************************************************
 * 
 * Function: Check_Lost_Right_Line
 * Author: Alper Ender
 * Description: Checks to see if the left sensor lost the line. Returns 1 if it did, else it returns 0
 * 
 **************************************************************/
boolean Check_Lost_Left_Line(void) {
  if (       leftSensor.sensorVals_T0[0] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[1] < OFF_LINE_THRESHOLD
         &&  leftSensor.sensorVals_T0[2] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[3] < OFF_LINE_THRESHOLD
         &&  leftSensor.sensorVals_T0[4] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[5] < OFF_LINE_THRESHOLD
         &&  leftSensor.sensorVals_T0[6] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[7] < OFF_LINE_THRESHOLD) {
    return true;
  }

  return false;
}


/**************************************************************
 * 
 * Function: Intersection
 * Author: Ben Wagner
 * Description: What to do when the vehicle reaches an intersection
 * 
 **************************************************************/
void Intersection(void) {
  Serial.println("INTERSECTION STATE");
  
  Stop_Vehicle();
  
  switch (random(3)) {
    case 0:   Go_Straight_OL(); break;
    case 1:   Right_Turn_OL();  break;
    case 2:   Left_Turn_OL();   break;
    default:  Go_Straight_OL(); break;
  }
  

}


/**************************************************************
 * 
 * Function: GoStraightOL
 * Author: Ben Wagner
 * Description: OPEN LOOP - Going Straight through the intersection
 * 
 **************************************************************/
void Go_Straight_OL(void) {
  unsigned long timer = millis();
  while (millis() - timer < STRAIGHTTIME) {
    BLF536.MotorRightPtr->setSpeed(115);
    BLF536.MotorLeftPtr->setSpeed(100);
  }
  return;
}


/**************************************************************
 * 
 * Function: GoStraightOL
 * Author: Ben Wagner
 * Description: OPEN LOOP - Turning right at the intersection
 * 
 **************************************************************/
void Right_Turn_OL(void) {
  unsigned long timer = millis();
  while (millis() - timer < RIGHTTURNTIME) {
    BLF536.MotorRightPtr->setSpeed(0);
    BLF536.MotorLeftPtr->setSpeed(100);
  }
}


/**************************************************************
 * 
 * Function: GoStraightOL
 * Author: Ben Wagner
 * Description: OPEN LOOP - Turning left at the intersection
 * 
 **************************************************************/
void Left_Turn_OL() {
  unsigned long timer = millis();
  while (millis() - timer < LEFTTURNTIME) {
    BLF536.MotorRightPtr->setSpeed(150);
    BLF536.MotorLeftPtr->setSpeed(80);
  }
}


void Follow_2_Line(void) {
  Serial.println("BOTH");
  BLF536.pos = (leftSensor.sensedVal + rightSensor.sensedVal) / 2L;
}


void Follow_Right(void) {
  Serial.println("RIGHT");
  BLF536.pos = rightSensor.sensedVal;
}


void Follow_Left(void) {
  Serial.println("LEFT");
  BLF536.pos = leftSensor.sensedVal;
}


void Open_Run(void) {
  Serial.println("NONE");
  BLF536.pos = (leftSensor.sensedVal + rightSensor.sensedVal) / 2L;
}

void Stop_Vehicle(void){
  BLF536.MotorRightPtr->setSpeed(0);
  BLF536.MotorLeftPtr->setSpeed(0);
  delay(1);
  BLF536.MotorRightPtr->setSpeed(100);
  BLF536.MotorLeftPtr->setSpeed(100);
  BLF536.MotorRightPtr->run(BACKWARD);
  BLF536.MotorLeftPtr->run(BACKWARD);
  delay(100);
  BLF536.MotorRightPtr->setSpeed(0);
  BLF536.MotorLeftPtr->setSpeed(0);
  BLF536.MotorRightPtr->run(FORWARD);
  BLF536.MotorLeftPtr->run(FORWARD);
}


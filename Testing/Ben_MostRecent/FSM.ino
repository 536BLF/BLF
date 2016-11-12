





// ------ THESE FUNCTIONS ARE NOT TESTED ------







/*
   Function: FSM
   Description: The finite state machine to determine which state the vehicle is currently in

   --- THIS FUNCTION IS CURRENTLY INCOMPLETE - 11/5 ---

*/
void FSM(void) {
  switch (state) {
    case INIT: break;
    case READ_2_LINE:     {
        Follow();
        break;
      }
    case READ_LEFT_LINE:  {
        Follow();
        break;
      }
    case READ_RIGHT_LINE: {
        Follow();
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


/*
   Function: Read_Sate
   Description: The function that determines the next state based on what the sensors are seeing.

   --- THIS FUNCTION IS CURRENTLY INCOMPLETE - 11/8 ---

*/
void Read_State(void) {
  boolean lostLeft, lostRight;
  int errorCalcSel;

  leftSensor.ReadAllSensors();
  rightSensor.ReadAllSensors();

  lostLeft = Check_Lost_Left_Line();
  lostRight = Check_Lost_Right_Line();

  if (lostLeft) leftSensor.Hold_Value();
  if (lostRight) rightSensor.Hold_Value();

  if (Check_Intersection()) {
    state = INTERSECTION;
  }

  if (lostLeft && lostRight) {
    state = READ_NO_LINE;
    errorCalcSel = 2;
  }

  if (lostLeft && !lostRight) {
    state = READ_RIGHT_LINE;
    errorCalcSel = 1;
  }

  if (!lostLeft && lostRight) {
    state = READ_LEFT_LINE;
    errorCalcSel = 0;
  }

  if (!lostLeft && !lostRight) {
    state = READ_2_LINE;
    errorCalcSel = 2;
  }

  leftSensor.Sensor_Error_Calc();
  rightSensor.Sensor_Error_Calc();

  BLF536.Total_Error_Calc(errorCalcSel);


}


/*
   Function: Check_Intersection
   Description: Checks the RIGHT sensor to see if it sees a corner. If it does, then it changes the FSM to the intersection state to handle this intersection
*/
int Check_Intersection(void) {
  if (  rightSensor.sensorVals_T0[4] > ON_LINE_THRESHOLD && rightSensor.sensorVals_T0[5] > ON_LINE_THRESHOLD
        && rightSensor.sensorVals_T0[6] > ON_LINE_THRESHOLD && rightSensor.sensorVals_T0[7] > ON_LINE_THRESHOLD) {
    return 1;
  }

  return 0;
}


/*
   Function: Check_Lost_Right_Line
   Description: checks to see if the analog1 sensor lost the line. Returns 1 if it did, else it returns 0
*/
int Check_Lost_Right_Line(void) {
  if (   rightSensor.sensorVals_T0[0] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[1] < OFF_LINE_THRESHOLD
         &&  rightSensor.sensorVals_T0[2] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[3] < OFF_LINE_THRESHOLD
         &&  rightSensor.sensorVals_T0[4] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[5] < OFF_LINE_THRESHOLD
         &&  rightSensor.sensorVals_T0[6] < OFF_LINE_THRESHOLD && rightSensor.sensorVals_T0[7] < OFF_LINE_THRESHOLD) {
    return 1;
  }

  return 0;
}


int Check_Found_Right_Line(void) {
  if (   rightSensor.sensorVals_T0[0] > ON_LINE_THRESHOLD || rightSensor.sensorVals_T0[1] > ON_LINE_THRESHOLD
         ||  rightSensor.sensorVals_T0[2] > ON_LINE_THRESHOLD || rightSensor.sensorVals_T0[3] > ON_LINE_THRESHOLD
         ||  rightSensor.sensorVals_T0[4] > ON_LINE_THRESHOLD || rightSensor.sensorVals_T0[5] > ON_LINE_THRESHOLD
         ||  rightSensor.sensorVals_T0[6] > ON_LINE_THRESHOLD || rightSensor.sensorVals_T0[7] > ON_LINE_THRESHOLD) {
    return 1;
  }

  return 0;
}


/*
   Function: Check_Lost_Left_Line
   Description: checks to see if the analog0 sensor lost the line. Returns 1 if it did, else it returns 0
*/
int Check_Lost_Left_Line(void) {
  if (   leftSensor.sensorVals_T0[0] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[1] < OFF_LINE_THRESHOLD
         &&  leftSensor.sensorVals_T0[2] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[3] < OFF_LINE_THRESHOLD
         &&  leftSensor.sensorVals_T0[4] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[5] < OFF_LINE_THRESHOLD
         &&  leftSensor.sensorVals_T0[6] < OFF_LINE_THRESHOLD && leftSensor.sensorVals_T0[7] < OFF_LINE_THRESHOLD) {
    return 1;
  }

  return 0;
}


int Check_Found_Left_Line(void) {
  if (   leftSensor.sensorVals_T0[0] > ON_LINE_THRESHOLD || leftSensor.sensorVals_T0[1] > ON_LINE_THRESHOLD
         ||  leftSensor.sensorVals_T0[2] > ON_LINE_THRESHOLD || leftSensor.sensorVals_T0[3] > ON_LINE_THRESHOLD
         ||  leftSensor.sensorVals_T0[4] > ON_LINE_THRESHOLD || leftSensor.sensorVals_T0[5] > ON_LINE_THRESHOLD
         ||  leftSensor.sensorVals_T0[6] > ON_LINE_THRESHOLD || leftSensor.sensorVals_T0[7] > ON_LINE_THRESHOLD) {
    return 1;
  }

  return 0;
}


void Intersection(void) {
  switch (random() % 3) {
    case 0: RightTurnOL(); break;
    case 1: GoStraightOL(); break;
    case 2: LeftTurnOL(); break;
  }
}

void GoStraightOL(void) {
  long timer = millis();
  while (millis() - timer < STRAIGHTTIME) {
    BLF536.motorDiffPWM = 0;
    BLF536.MotorDiff();
  }
  return;
}

void RightTurnOL(void) {
  long timer = millis();
  while (millis() - timer < RIGHTTURNTIME) {
    BLF536.MotorRightPtr->setSpeed(5);
    BLF536.MotorLeftPtr->setSpeed(255);
  }
}

void LeftTurnOL() {
  long timer = millis();
  while (millis() - timer < LEFTTURNTIME) {
    BLF536.MotorRightPtr->setSpeed(255);
    BLF536.MotorLeftPtr->setSpeed(100);
  }
}
void Follow(void) {
}

void Open_Run(void) {

}

int Controller(long error, int controllertype){
  
  
  
  
  }

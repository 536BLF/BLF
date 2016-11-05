#include "Macros.h"
#include <AFMotor.h>

class car
{
private:
   int pwmSetSpeed=SETSPEED;
   int motorDiffPWM;
   double Kprop;
   double refPoint;
   
   int analog0[NUM_SENSORS];
   int analog1[NUM_SENSORS];
   int oldAnalog0[NUM_SENSORS];
   int oldAnalog1[NUM_SENSORS];
   int sensorMin = MAX_SENSOR_VAL;        // minimum sensor value
   int sensorMax = MIN_SENSOR_VAL;        // maximum sensor value
   unsigned long timerVal;
   void Mux_Select(int);
   void Array_Copy(int);
   int Read_Average(int, int);
   
public:
   float error0;
   float error1;
   float totalError;
   unsigned long lineVal0;
   unsigned long lineVal1;
   unsigned long initLineVal0;
   unsigned long initLineVal1;
   void MotorDiff();
   void sysId();
   void Init_Sensors();
   void ReadAllSensors(int);
   void Get_Line_Value(int);
   void Total_Error_Calc(int);
   void Sensor_Error_Calc(int);
   void Print_Sensor_Values(int);
   car();
};

car::car()
{
  Kprop = KPROP;
  totalError = 0;
  refPoint = 0;
<<<<<<< HEAD
  lineVal0 = 0;
  lineVal1 = 0;
=======
  AF_DCMotor motorRight(1, MOTOR12_64KHZ);
  AF_DCMotor motorLeft(2, MOTOR12_64KHZ);

  
>>>>>>> 4527f48a4ac592901504748ccdb9c0545f62f10f
}

void car::MotorDiff()
{
int RightSpeed;
int LeftSpeed;

//   assume diffvalue>0 as right turn and <0 is a right turn
   LeftSpeed=this->pwmSetSpeed-(this->motorDiffPWM/2);
   RightSpeed=this->pwmSetSpeed+(this->motorDiffPWM/2);
   // 
<<<<<<< HEAD
   //digitalWrite(DIRECTIONPIN1,HIGH);
   //digitalWrite(DIRECTIONPIN2, LOW);
   //analogWrite(SPEEDPIN1,LeftSpeed);// write speed for motor1
   //analogWrite(SPEEDPIN1,LeftSpeed); // write speed for motor2 
=======
   motorRight.setSpeed(RightSpeed);
   motorLeft.setSpeed(LeftSpeed);

   motorRight.run(FORWARD);
   delay (MOTORDELAY)
   motorLeft.run(FORWARD);
   
//   digitalWrite(DIRECTIONPIN1,HIGH);
//   digitalWrite(DIRECTIONPIN2, LOW);
//   analogWrite(SPEEDPIN1,LeftSpeed);// write speed for motor1
//   analogWrite(SPEEDPIN2,RightSpeed); // write speed for motor2
 
>>>>>>> 4527f48a4ac592901504748ccdb9c0545f62f10f
}

void car::sysId()
{
  if(millis()<ZEROREFTIME) refPoint = 0; //Reference point is zero until time specified, then goes to 1. (step function)
  else refPoint = 1;

  motorDiffPWM = Kprop*totalError; //Proportional controller, error times a constant = motor differential
}

void car::Init_Sensors(void){
  int i, sensorVal0, sensorVal1;
  
  pinMode(PIN_S2, OUTPUT);            // Select S2 - PIN A5
  pinMode(PIN_S1, OUTPUT);            // Select S1 - PIN A4
  pinMode(PIN_S0, OUTPUT);            // Select S0 - PIN A3
  pinMode(PIN_LED_ARDUINO, OUTPUT);   // Arduino LED

    digitalWrite(PIN_LED_ARDUINO, HIGH);        // Arduino LED ON during calibration
  
  while (millis() < SECONDS_8) {              // calibrate during the first 8 seconds
    
    i++;
    Mux_Select(floor(i/MULTIPLIER));          // Cycling through the mux every 1 second

    sensorVal0 = analogRead(PIN_ANALOG_0);    // Reading the sensor
    sensorVal1 = analogRead(PIN_ANALOG_1);

    if (sensorVal0 > sensorMax) sensorMax = sensorVal0;   // record the maximum sensor value
    if (sensorVal1 > sensorMax) sensorMax = sensorVal1;

    if (sensorVal0 < sensorMin) sensorMin = sensorVal0;   // record the minimum sensor value
    if (sensorVal1 < sensorMin) sensorMin = sensorVal1;
  }
  
  this->ReadAllSensors(BLACK_LINE);
  
  Get_Line_Value(PIN_ANALOG_0); 
  this->initLineVal0 = this->lineVal0;
  Get_Line_Value(PIN_ANALOG_1); 
  this->initLineVal1 = this->lineVal1;

  digitalWrite(PIN_LED_ARDUINO, LOW);
}

void car::Mux_Select(int select){

  switch(select){
    case 0: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, LOW);  break;    // 000
    case 1: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, HIGH);  break;   // 001
    case 2: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, LOW);  break;   // 010
    case 3: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, HIGH);  break;  // 011
    case 4: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, LOW);  break;   // 100
    case 5: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, HIGH);  break;  // 101
    case 6: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, LOW);  break;  // 110
    case 7: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, HIGH);  break; // 111
    default: return;
  }

  delayMicroseconds(4);   // The delay necessary for the mux to switch
}

void car::ReadAllSensors(int invert){
  int tmp0, tmp1, i;

  this->Array_Copy(PIN_ANALOG_0);                                   // Copying the current values to the old values to have something to hold onto.
  this->Array_Copy(PIN_ANALOG_1);
  
  for(i=0 ; i<NUM_SENSORS ; i++){
    
    Mux_Select(i);                                            // Cycling through the mux
    
    tmp0 = this->Read_Average(PIN_ANALOG_0, NUM_SAMPLES);     // Obtaining an average of the readings
    tmp1 = this->Read_Average(PIN_ANALOG_1, NUM_SAMPLES);

    tmp0 = map(tmp0, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Obtaining the sensor values between the calibrated min and max and mapping them between 0 - 1023
    tmp1 = map(tmp1, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    tmp0 = constrain(tmp0, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Constraining the values between 0 - 1023
    tmp1 = constrain(tmp1, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    if(invert){
      tmp0 = MAX_SENSOR_VAL - tmp0;                           // If we are reading a white line, then the values will invert
      tmp1 = MAX_SENSOR_VAL - tmp1;                           // NO MATTER WHAT: ~0 -> Off the line    ,     ~1023 -> On the line
    }
    
    this->analog0[i] = tmp0; 
    this->analog1[i] = tmp1;

  }
}


void car::Array_Copy(int select){
  int i;
  switch(select){
    case PIN_ANALOG_0:

      for(i=0 ; i<NUM_SENSORS ; i++){
        this->oldAnalog0[i] = this->analog0[i];
      }
      break;

    case PIN_ANALOG_1:

      for(i=0 ; i<NUM_SENSORS ; i++){
        this->oldAnalog1[i] = this->analog1[i];
      }
      break;
    
  }
}


int car::Read_Average(int select, int num_samples){
  int i;
  int total = 0;

  switch(select){
    case PIN_ANALOG_0: 
      
      for(i=0 ; i<num_samples ; i++){
        total += analogRead(PIN_ANALOG_0);    // Total value from all the samples
      }
      break;
            
    case PIN_ANALOG_1: 
    
      for(i=0 ; i<num_samples ; i++){
        total += analogRead(PIN_ANALOG_1);
      }
      break;

    default: return 0;
  }

  return (total/NUM_SAMPLES);                 // Returnin the average from the number of samples
}


void car::Get_Line_Value(int select){
  int i;
  unsigned long total = 0;
  unsigned long sum = 0;
  unsigned long out = 0;

  switch(select){
  case PIN_ANALOG_0: 
  
    for(i=0 ; i<NUM_SENSORS ; i++){
      total += this->analog0[i] * (i * MULTIPLIER);   // Numerator of Equation
      sum += this->analog0[i];                        // Denominator of Equation
    }
    
    out = total/sum;                            // Dividing numerator by denominator
    this->lineVal0 = out;
    
    break;
      
  case PIN_ANALOG_1: 
  
    for(i=0 ; i<NUM_SENSORS ; i++){
      total += this->analog1[i] * (i * MULTIPLIER);
      sum += this->analog1[i];
    }
    
    out = total/sum;                            // Dividing numerator by denominator
    this->lineVal1 = out;
    
    break;
      
  default: break;
 }
}

void car::Total_Error_Calc(int select){
  switch(select){
    case 0:
      this->totalError = error0;                // When the line for sensor 1 has disappeared

    case 1:
      this->totalError = error1;                // When the line for sensor 0 has disappeared

    case 2:
      this->totalError = ((-this->error0) + this->error1)/2;   // Average of the sensor errors
  }
}


void car::Sensor_Error_Calc(int select){
  
   switch(select){
    case PIN_ANALOG_0:
    
      Get_Line_Value(PIN_ANALOG_0);                        // Get the line value between 0 - 7000
      this->error0 = ((float)this->lineVal0 - (float)this->initLineVal0) / (float)1000;   // Get the error of the sensor in cm
      break;

    case PIN_ANALOG_1:

      Get_Line_Value(PIN_ANALOG_1);
      this->error1 = ((float)this->lineVal1 - (float)this->initLineVal0 ) / (float)1000;
      break;

    default: break;
   }
}


void car::Print_Sensor_Values(int select){
  int i;
  
  switch(select){
    case PIN_ANALOG_0:
      
      Serial.print("ANALOG 0 SENSORS:");
      for(i=0 ; i<NUM_SENSORS ; i++){
        Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(this->analog0[i]);
      }
      Serial.println("");
      break;
    
    case PIN_ANALOG_1:
      
      Serial.print("ANALOG 1 SENSORS");
      for(i=0 ; i<NUM_SENSORS ; i++){
        Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(this->analog1[i]);
      }
      Serial.println("");
      break;
    
    default: break;
  }
}


car BLF536;

void setup(){
  Serial.begin(9600);
  //pinMode(DIRECTIONPIN1, INPUT);
  //pinMode(DIRECTIONPIN2, INPUT);
  //pinMode(SPEEDPIN1, OUTPUT);
  //pinMode(SPEEDPIN2, OUTPUT);

  BLF536.Init_Sensors();
}

void loop(){
  BLF536.ReadAllSensors(BLACK_LINE);
  BLF536.sysId();
  
  // --- Uncomment any of these below if you want to see specific outputs --- //
  
  /*
  Serial.print("Analog0INIT:\t"); Serial.print(BLF536.initLineVal0); Serial.print("\t");
  Serial.print("Analog1INIT:\t"); Serial.print(BLF536.initLineVal1); Serial.print("\t");
  BLF536.Sensor_Error_Calc(PIN_ANALOG_0);
  Serial.print("Analog0:\t"); Serial.print(BLF536.error0); Serial.print("\t");
  BLF536.Sensor_Error_Calc(PIN_ANALOG_1);
  Serial.print("Analog1:\t"); Serial.print(BLF536.error1); Serial.print("\t");
  BLF536.Total_Error_Calc(2);
  Serial.print("Total:\t"); Serial.println(BLF536.totalError);
  */
  
  /*
  BLF536.Print_Sensor_Values(PIN_ANALOG_0);
  BLF536.Print_Sensor_Values(PIN_ANALOG_1);
  */
  
  /*
  BLF536.Get_Line_Value(PIN_ANALOG_0);
  Serial.print("Analog0:\t"); Serial.print(BLF536.lineVal0); Serial.print("\t");
  BLF536.Get_Line_Value(PIN_ANALOG_1);
  Serial.print("Analog1:\t"); Serial.println(BLF536.lineVal1);
  */
 
  
}

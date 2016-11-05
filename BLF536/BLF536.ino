#include "Macros.h"
#include <AFMotor.h>

class car
{
private:
   int pwmSetSpeed=SETSPEED;
   int motorDiffPWM;
   double Kprop;
   double refPoint;
   
   volatile int analog0[NUM_SENSORS];
   volatile int analog1[NUM_SENSORS];
   int oldAnalog0[NUM_SENSORS];
   int oldAnalog1[NUM_SENSORS];
   int sensorMin = MAX_SENSOR_VAL;        // minimum sensor value
   int sensorMax = MIN_SENSOR_VAL;        // maximum sensor value
   unsigned long lineVal0;
   unsigned long lineVal1;
   float error0;
   float error1;
   float totalError;
   unsigned long timerVal;
   void Mux_Select(int select);
public:
   void MotorDiff();
   void sysId();
   void Init_Sensors();
   void ReadAllSensors(int);
   car();
};

car::car()
{
  Kprop = KPROP;
  totalError = 0;
  refPoint = 0;
  AF_DCMotor motorRight(1, MOTOR12_64KHZ);
  AF_DCMotor motorLeft(2, MOTOR12_64KHZ);

  
}

void car::MotorDiff()
{
int RightSpeed;
int LeftSpeed;

//   assume diffvalue>0 as right turn and <0 is a right turn
   LeftSpeed=this->pwmSetSpeed-(this->motorDiffPWM/2);
   RightSpeed=this->pwmSetSpeed+(this->motorDiffPWM/2);
   // 
   motorRight.setSpeed(RightSpeed);
   motorLeft.setSpeed(LeftSpeed);

   motorRight.run(FORWARD);
   delay (MOTORDELAY)
   motorLeft.run(FORWARD);
   
//   digitalWrite(DIRECTIONPIN1,HIGH);
//   digitalWrite(DIRECTIONPIN2, LOW);
//   analogWrite(SPEEDPIN1,LeftSpeed);// write speed for motor1
//   analogWrite(SPEEDPIN2,RightSpeed); // write speed for motor2
 
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

  Array_Copy(PIN_ANALOG_0);                                   // Copying the current values to the old values to have something to hold onto.
  Array_Copy(PIN_ANALOG_1);
  
  for(i=0 ; i<NUM_SENSORS ; i++){
    
    Mux_Select(i);                                            // Cycling through the mux
    
    tmp0 = Read_Average(PIN_ANALOG_0, NUM_SAMPLES);           // Obtaining an average of the readings
    tmp1 = Read_Average(PIN_ANALOG_1, NUM_SAMPLES);

    tmp0 = map(tmp0, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Obtaining the sensor values between the calibrated min and max and mapping them between 0 - 1023
    tmp1 = map(tmp1, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    tmp0 = constrain(tmp0, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Constraining the values between 0 - 1023
    tmp1 = constrain(tmp1, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    if(invert){
      tmp0 = MAX_SENSOR_VAL - tmp0;                           // If we are reading a white line, then the values will invert
      tmp1 = MAX_SENSOR_VAL - tmp1;                           // NO MATTER WHAT: ~0 -> Off the line    ,     ~1023 -> On the line
    }
    
    analog0[i] = tmp0; 
    analog1[i] = tmp1;

  }
}

car BLF536;

void setup(){
  Serial.begin(9600);
  pinMode(DIRECTIONPIN1, INPUT);
  pinMode(DIRECTIONPIN2, INPUT);
  pinMode(SPEEDPIN1, OUTPUT);
  pinMode(SPEEDPIN2, OUTPUT);

  BLF536.Init_Sensors();
}

void loop(){
  BLF536.ReadAllSensors(BLACK_LINE);
  BLF536.sysId();
}

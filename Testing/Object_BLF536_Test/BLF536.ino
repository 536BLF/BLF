#include "Macros.h"
#include <AFMotor.h>

class Sensor{
  public:
     int whiteLine;
     int sensorVals_T0[NUM_SENSORS];
     int sensorVals_T1[NUM_SENSORS];
     int sensorMin = MAX_SENSOR_VAL;        // minimum sensor value
     int sensorMax = MIN_SENSOR_VAL;        // maximum sensor value
     unsigned long timerVal;
     uint8_t pin;
     float error;
     float totalError;
     unsigned long lineVal;
     unsigned long initLineVal;

     // Functions
     void MotorDiff();
     void sysId();
     void Init_Sensors();
     void ReadAllSensors(void);
     void Get_Line_Value(void);
     void Sensor_Error_Calc(void);
     void Print_Sensor_Values(void);
     void Calibrate(void);
     void Mux_Select(int);
     void Array_Copy(void);
     int Read_Average(void);

     // Constructor
     Sensor(const uint8_t&, int);

};

Sensor leftSensor(PIN_ANALOG_0, BLACK_LINE);
Sensor rightSensor(PIN_ANALOG_1, BLACK_LINE);

class car
{
  public:
    int totalError;
    int pwmSetSpeed=SETSPEED;
    int motorDiffPWM;
    double Kprop;
    double refPoint;
    void MotorDiff(void);
    void sysId(void);
    void Total_Error_Calc(int);
    
    car();
};

car::car()
{
  Kprop = KPROP;
  refPoint = 0;
  AF_DCMotor motorRight(1, MOTOR12_64KHZ);
  AF_DCMotor motorLeft(2, MOTOR12_64KHZ);
  //rightSensor.a;
}

void car::MotorDiff()
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


void car::Total_Error_Calc(int select){
  switch(select){
    case 0:
      this->totalError = leftSensor.error;                // When the line for sensor 1 has disappeared

    case 1:
      this->totalError = rightSensor.error;                // When the line for sensor 0 has disappeared

    case 2:
      this->totalError = ((-leftSensor.error) + rightSensor.error)/2;   // Average of the sensor errors
  }
}


void car::sysId()
{
  if(millis()<ZEROREFTIME) refPoint = 0; //Reference point is zero until time specified, then goes to 1. (step function)
  else refPoint = 1;
  
  motorDiffPWM = Kprop*totalError; //Proportional controller, error times a constant = motor differential
}

car BLF536;

void setup(){
  Serial.begin(9600);

  leftSensor.Init_Sensors();
  rightSensor.Init_Sensors();
}

void loop(){
  leftSensor.ReadAllSensors();
  rightSensor.ReadAllSensors();
  //BLF536.sysId();
  
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
  
 
  leftSensor.Print_Sensor_Values();
  rightSensor.Print_Sensor_Values();
  Serial.println("");


  
  /*
  BLF536.Get_Line_Value(PIN_ANALOG_0);
  Serial.print("Analog0:\t"); Serial.print(BLF536.lineVal0); Serial.print("\t");
  BLF536.Get_Line_Value(PIN_ANALOG_1);
  Serial.print("Analog1:\t"); Serial.println(BLF536.lineVal1);
  */
 
  
}

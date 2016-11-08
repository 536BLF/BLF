#include "Macros.h"
#include <AFMotor.h>

class Sensor{
  public:
    //variables
    int whiteLine;
    int sensorVals_T0[NUM_SENSORS];
    int sensorVals_T1[NUM_SENSORS];
    int sensorVals_T2[NUM_SENSORS];
    int sensorMin;                  // minimum sensor value
    int sensorMax;                  // maximum sensor value
    int tmp;
    uint8_t pin;
    float error;
    float totalError;
    unsigned long lineVal;
    unsigned long initLineVal;
    unsigned long timerVal;

    
    // Functions
    void MotorDiff();
    void sysId();
    void Init_Sensors();
    void ReadAllSensors(void);
    void Get_Line_Value(void);
    void Sensor_Error_Calc(void);
    void Calibrate(void);
    void Mux_Select(int);
    void Array_Copy(void);
    int Read_Average(void);
    
    // Constructor
    Sensor(const uint8_t&, int);

};

Sensor leftSensor(PIN_ANALOG_0, BLACK_LINE);
Sensor rightSensor(PIN_ANALOG_1, BLACK_LINE);

class Tester{
  public:
    long int timerVal;
  
    void Print_Sensor_Values(void);
    void Print_Line_Values(void);
    void Print_Total_Error(void);
    void Start_Timer(void);
    void Display_Timer(void);

    Tester();
};

Tester tester;


class Car
{
  public:

    // Variables
    float totalError;
    int pwmSetSpeed=SETSPEED;
    int motorDiffPWM;
    double Kprop;
    double refPoint;

    // Functions
    void MotorDiff(void);
    void sysId(void);
    void Total_Error_Calc(int);

    // Constructor
    Car();
};

Car BLF536;


void setup(){
  Serial.begin(9600);

  leftSensor.Init_Sensors();
  rightSensor.Init_Sensors();
}


void loop(){
  tester.Start_Timer();
  
  leftSensor.ReadAllSensors();
  rightSensor.ReadAllSensors();
  
  //BLF536.sysId();


  
  // --- Uncomment any of these below if you want to see specific outputs --- //

  // tester.Print_Total_Error();
  // tester.Print_SensorValues();
  // tester.Print_Line_Val();
  
  tester.Display_Timer();
}

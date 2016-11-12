#include "Macros.h"
//#include <AFMotor.h>

enum {INIT, READ_2_LINE, READ_LEFT_LINE, READ_RIGHT_LINE, READ_NO_LINE, INTERSECTION} state = INIT;


class Sensor {
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
    void Hold_Value(void);

    // Constructor
    Sensor(const uint8_t&, int);

};

Sensor leftSensor(PIN_ANALOG_0, BLACK_LINE);
Sensor rightSensor(PIN_ANALOG_1, BLACK_LINE);


class Tester {
  public:
    unsigned long timerVal;
    unsigned long sample = 0;

    void Print_Sensor_Values(void);
    void Print_Line_Values(void);
    void Print_Total_Error(void);
    void Start_Timer(void);
    void Display_Timer(void);
    void System_Id(void);

    Tester();
};

Tester tester;


class Car
{
  public:

    // Variables
    boolean sysIdCompleted = false;
    int pwmSetSpeed = SETSPEED;
    int motorDiffPWM;
    float totalError;
    unsigned long timer = 0;
    unsigned long delta = 0;
    double Kprop;
    double refPoint;

    // Functions
    void MotorDiff(void);
    void sysId(void);
    void Total_Error_Calc(int);
    void Sample_Time(void);
    void System_Identification(void);

    // Constructor
    Car();
};

Car BLF536;


/*
* Function: setup
* Description: Initializes our loop
*/
void setup() {
  Serial.begin(9600);

  leftSensor.Init_Sensors();
  rightSensor.Init_Sensors();

  BLF536.System_Identification();
}


/*
* Function: loop
* Description: Our main while loop that runs forever
*/
void loop() {
  // Serial.println("here");
  // tester.Start_Timer();

  BLF536.Sample_Time();

  BLF536.System_Identification();

  leftSensor.ReadAllSensors();
  rightSensor.ReadAllSensors();

  leftSensor.Sensor_Error_Calc();
  rightSensor.Sensor_Error_Calc();

  BLF536.Total_Error_Calc(2);


  // --- Uncomment any of these below if you want to see specific outputs --- //

  tester.System_Id();
  // tester.Print_Total_Error();
  // tester.Print_Sensor_Values();
  // tester.Print_Line_Val();

  // tester.Display_Timer();
}

// Libraries
#include "Macros.h"
#include <AFMotor.h>

// Global Variables
enum {INIT, READ_2_LINE, READ_LEFT_LINE, READ_RIGHT_LINE, READ_NO_LINE, INTERSECTION} state = INIT;


class Sensor{
  public:
  
    // Variables
    int           whiteLine;                    // Whether we are reading a black or white line
    int           sensorVals_T0[NUM_SENSORS];   // Sensor values for T = 0
    int           sensorVals_T1[NUM_SENSORS];   // Sensor valeus for T = -1 (Sensor values from one time interval before
    int           sensorVals_T2[NUM_SENSORS];   // Sensor valeus for T = -2 (Sensor values from two time intervals before
    int           sensorMin;                    // Minimum sensor value
    int           sensorMax;                    // Maximum sensor value
    int           tmp;                          // Temporary value for sensor readings
    uint8_t       pin;                          // Analog input pin select for the sensor
    float         error;                        // Error value for the sensor - between -3.5 and 3.5 CENTIMETERS
    unsigned long lineVal;                      // The line value calculated from the current sensor values - between 0 and 7000
    unsigned long initLineVal;                  // Initialized line value based on where the car begins
    
    // Functions
    void MotorDiff();
    void sysId();
    void Init_Sensors();
    void Read_All_Sensors(void);
    void Get_Line_Value(void);
    void Sensor_Error_Calc(void);
    void Calibrate(void);
    void Mux_Select(int);
    void Array_Copy(void);
    void Hold_Value(void);
    int  Read_Average(void);
    
    // Constructor
    Sensor(const uint8_t&, int);

};

// Initializing the left sensor and the right sensor arrays
Sensor leftSensor(PIN_ANALOG_0, BLACK_LINE);
Sensor rightSensor(PIN_ANALOG_1, BLACK_LINE);


class Tester{
  public:
  
    // Variables
    unsigned long timerVal;     // Timer value for testing the time it takes to complete a specific task
    unsigned long sample = 0;   // Sample counter for system identification
    
    // Functions
    void Print_Sensor_Values(void);
    void Print_Line_Values(void);
    void Print_Total_Error(void);
    void Start_Timer(void);
    void Display_Timer(void);
    void SystemId(void);
    
    // Constructor
    Tester();
};

// Initializing the tester class
Tester tester;


class Car
{
  public:

    // Variables
    boolean       impulseBegin      = false;      // Whether or not we have began the impulse for the system identification
    boolean       impulseEnd        = false;      // Whether or not we have completed the impulse for the system identification
    int           pwmSetSpeed       = SETSPEED;   // Setting the PWM speed
    int           motorDiffPWM;                   // The motor differential PWM
    unsigned long deltaTime;                      // How long to hold the impulse for the system identification
    unsigned long beginTime         = 0;          // When to begin the impulse for the system identification
    unsigned long timer             = 0;          // Timer to ensure that the each loop runs T second intervals
    float         totalError;                     // The total error of the vehicle from its calibrated location
    AF_DCMotor    *MotorRightPtr;
    AF_DCMotor    *MotorLeftPtr;

    // Functions
    void MotorDiff(void);
    void sysId(void);
    void Total_Error_Calc(int);
    void Sample_Time(void);
    void System_Identification(void);
    void Read_Sensors_And_Obtain_Errors(void);

    // Constructor
    Car();
};

// Initializing our Car and Motors
Car BLF536;
AF_DCMotor MotorLeft(4);
AF_DCMotor MotorRight(3);


void setup(){
  Serial.begin(9600);

  // --- INIT MOTORS --- //
  BLF536.MotorRightPtr=&MotorRight;
  BLF536.MotorLeftPtr=&MotorLeft;

  // --- INIT SENSORS --- //
  leftSensor.Init_Sensors();
  rightSensor.Init_Sensors();

  // --- INIT COMPUTER --- //
  BLF536.System_Identification();
  BLF536.Sample_Time();
}


void loop(){
  // tester.Start_Timer();    // Timer Begin


  // --- HOLDS HERE TO GET EXACTLY THE T SECOND INTERVAL --- //
  BLF536.Sample_Time();

  // --- COMPUTER --- //
  BLF536.System_Identification();

  // --- SENSORS --- //
  BLF536.Read_Sensors_And_Obtain_Errors();

  // --- MOTOR CONTROL --- //
  BLF536.motorDiffPWM=100;
  BLF536.MotorDiff();

  
  // --- TESTING: Uncomment any of these below if you want to see specific outputs to the serial port --- //
  tester.SystemId();
  // tester.Print_Total_Error();
  // tester.Print_Sensor_Values();
  // tester.Print_Line_Values();
  
  // tester.Display_Timer();   // Displays Timer
}

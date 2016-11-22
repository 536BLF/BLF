// Libraries
#include "Macros.h"
#include <AFMotor.h>
#include <TimerOne.h>

volatile unsigned long counter = 1;
volatile unsigned long counterHold = 0;

// Global Variables
enum {INIT, READ_2_LINE, READ_LEFT_LINE, READ_RIGHT_LINE, READ_NO_LINE, INTERSECTION} state = INIT;


class Sensor{
  public:
  
    // Variables
    int       whiteLine;                    // Whether we are reading a black or white line
    uint8_t   pin;                          // Analog input pin select for the sensor
    volatile double    sensorVals_T0[NUM_SENSORS];   // Sensor values for T = 0
    volatile double    sensorVals_T1[NUM_SENSORS];   // Sensor valeus for T = -1 (Sensor values from one time interval before
    volatile double    sensorVals_T2[NUM_SENSORS];   // Sensor valeus for T = -2 (Sensor values from two time intervals before
    volatile double    sensorMin;                    // Minimum sensor value
    volatile double    sensorMax;                    // Maximum sensor value
    volatile double    tmp;                          // Temporary value for sensor readings
    volatile double    error;                        // Error value for the sensor - between -3.5 and 3.5 CENTIMETERS
    volatile double    lineVal;                      // The line value calculated from the current sensor values - between 0 and 7000
    volatile double    initLineVal;                  // Initialized line value based on where the car begins
    volatile double    sensedVal;
    volatile double    sensedValHold    = 0;
    
    // Functions
    void Init_Sensors();
    void Read_All_Sensors(void);
    void Get_Line_Value(void);
    void Sensor_Calc(void);
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
    volatile unsigned long timer             = 0;            // Timer to ensure that the each loop runs T second intervals 
    volatile double        setPoint          = 0;            // Set point for system identification: 1 for system identification, 0 for regular control
    volatile double        pwmSetSpeed       = SETSPEED;     // Setting the PWM speed   
    volatile double        pos;                              // The position of the vehicle in reference to the set point
    volatile double        posHold            = 0;            // Deadband
    volatile double        totalError;                       // The total error of the vehicle from its set point
    volatile double        totalErrorHold[HOLD_AMOUNT];      // Holding the total error amounts
    volatile double        motorDiffPWM;                     // The motor differential PWM
    volatile double        motorDiffPWMHold[HOLD_AMOUNT];    // Holding the motor differential ammounts
    AF_DCMotor    *MotorRightPtr;
    AF_DCMotor    *MotorLeftPtr;

    // Functions
    void MotorDiff(void);
    void sysId(void);
    void Total_Error_Calc(void);
    void Sample_Time(void);
    void Read_Sensors_And_Obtain_Errors(void);
    void Controller(void);
    void Filter(void);

    // Constructor
    Car();
};

// Initializing our Car and Motors
Car BLF536;
AF_DCMotor MotorLeft(4);
AF_DCMotor MotorRight(3);


void setup(){
  Serial.begin(115200);

  // --- INIT MOTORS --- //
  BLF536.MotorRightPtr=&MotorRight;
  BLF536.MotorLeftPtr=&MotorLeft;

  // --- INIT SENSORS --- //
  leftSensor.Init_Sensors();
  rightSensor.Init_Sensors();

  Timer1.initialize(10000L);         // initialize timer1, 
  Timer1.attachInterrupt(callback, 10000L);

  // --- INIT COMPUTER --- //
  //BLF536.timer = micros() + 10000;
}


void loop(){
  

  // --- HOLDS HERE TO GET EXACTLY THE T SECOND INTERVAL --- //
  // BLF536.Sample_Time();


  
  if(counter!=counterHold){

  // --- THIS SHOULD BE UNCOMMENTED IF NOT USING INTERRUPTS --- //
  /* 
  if(BLF536.timer < micros()){
  
    // --- SENSORS --- // 
    BLF536.Read_Sensors_And_Obtain_Errors();
    BLF536.timer = micros() + 10000L;
  */
    
    // --- CONTROLLER --- //
    BLF536.Controller();


    // --- MOTOR CONTROL --- //
    BLF536.MotorDiff();


    // --- TESTING: Uncomment any of these below if you want to see specific outputs to the serial port --- //
    
    // Serial.println(BLF536.totalError);
    // Serial.println(BLF536.motorDiffPWM);
    
    tester.SystemId();
    // tester.Print_Total_Error();
    // tester.Print_Line_Values();
    // tester.Print_Sensor_Values();


    // --- DISPLAYING TIMERS --- //
    // tester.Display_Timer();  // Displays Timer
    // tester.Start_Timer();    // Begins Timer


    // Updating counter to look for next interrupt
    counterHold = counter;
  }
    
}

//#include "Car.ino"
#include "Macros.h"

class Car{
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
    AF_DCMotor *MotorRightPtr;
    AF_DCMotor *MotorLeftPtr;

    // Functions
    void MotorDiff(void);
    void sysId(void);
    void Total_Error_Calc(int);
    void Sample_Time(void);
    void System_Identification(void);

    // Constructor
    Car();
};



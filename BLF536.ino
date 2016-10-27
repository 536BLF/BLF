#define SETSPEED 175 //arbitrary
#define ZEROREFTIME 1000 //Delay before unit step kicks in
#define KPROP 100; //Proportional controller constant

class car
{
private:
   int pwmSetSpeed;
   int motorDiffPWM;
   double Kprop;
   double refPoint;
   double error;
   double sensVal;
public:
   void MotorDiff();
   void sysId();
   void getVal();
   car();
};

car::car()
{
  Kprop = KPROP;
  sensVal = 0;
  error = 0;
  refPoint = 0;
}

void car::MotorDiff()
{
//MotorDiff code here
}

void car::sysId()
{
  if(millis()<ZEROREFTIME) refPoint = 0; //Reference point is zero until time specified, then goes to 1. (step function)
  else refPoint = 1;

  motorDiffPWM = Kprop*error; //Proportional controller, error times a constant = motor differential
}

//Gets sensed position from the sensors and calculates error
void car::getVal()
{
  error = refPoint - sensVal; //error is the reference point minus the sensed value
  Serial.println(sensVal);
}

car BLF536;

void setup(){
  Serial.begin(9600);
}

void loop(){
  BLF536.getVal();
  BLF536.sysId();
}

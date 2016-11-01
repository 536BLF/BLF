#define SETSPEED 175 //arbitrary
#define ZEROREFTIME 1000 //Delay before unit step kicks in
#define KPROP 100; //Proportional controller constant
#define DIRECTIONPIN1 7
#define DIRECTIONPIN2 10
// motor input1 and input2 they should be PWM signals
#define SPEEDPIN1 3
#define SPEEDPIN2 14
// motor output1 and 2   those pins are decided by pin configuration in Datasheet
class car
{
private:
   int pwmSetSpeed=SETSPEED;
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
int RightSpeed;
int LeftSpeed;

//   assume diffvalue>0 as right turn and <0 is a right turn
   LeftSpeed=this->pwmSetSpeed-(this->motorDiffPWM/2);
   RightSpeed=this->pwmSetSpeed+(this->motorDiffPWM/2);
   // 
   digitalWrite(DIRECTIONPIN1,HIGH);
   digitalWrite(DIRECTIONPIN2, LOW);
   analogWrite(SPEEDPIN1,LeftSpeed);// write speed for motor1
   analogWrite(SPEEDPIN1,LeftSpeed); // write speed for motor2 
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
  pinMode(DIRECTIONPIN1, INPUT);
  pinMode(DIRECTIONPIN2, INPUT);
  pinMode(SPEEDPIN1, OUTPUT);
  pinMode(SPEEDPIN2, OUTPUT);
}

void loop(){
  BLF536.getVal();
  BLF536.sysId();
}

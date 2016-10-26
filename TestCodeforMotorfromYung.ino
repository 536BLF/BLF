#define diffvalue 15
// diffvalue
#define motor1pin 7
#define motor2pin 10
// motor input1 and input2 they should be PWM signals
#define speed1pin 3
#define speed2pin 14
// motor output1 and 2   those pins are decided by pin configuration in Datasheet

int Motorbasespeed = 127;
//a base value to hold the current speed value. 0-255 set 127= base speed
//255= full speed

void setup()
{
pinMode(diffvalue, INPUT);
// set diffvalue as input
pinMode(motor1pin, INPUT);
pinMode(motor2pin, INPUT);
pinMode(speed1pin, OUTPUT);
pinMode(speed2pin, OUTPUT);
// set those pins as output
}

void loop()
{
analogWrite(speed1pin,Motorbasespeed );// write speed for motor1

if(digitalRead(diffvalue>0)){    //   assume diffvalue>0 as right turn 
   digitalWrite(motor1pin,HIGH);
   digitalWrite(motor2pin, LOW);
   }
analogWrite(speed2pin,Motorbasespeed ); // write speed for motor2 
if(digitalRead(diffvalue<0)){    //  assume diffvalue<0 as ledt turn 
   digitalWrite(motor1pin,LOW);
   digitalWrite(motor2pin, HIGH);
   }

}
//  PWM 128=(PWMR-PWML)/2
//  K*E= PWMR-PWML

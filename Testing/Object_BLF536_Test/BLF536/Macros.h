#define PIN_ANALOG_0 (A0)
#define PIN_ANALOG_1 (A1)

#define PIN_LED_0 (9)
#define PIN_LED_1 (8)

#define PIN_S2 (A5)
#define PIN_S1 (A4)
#define PIN_S0 (A3)

#define PIN_LED_ARDUINO (13)

#define MULTIPLIER (1000UL)

#define MIN_SENSOR_VAL (0)
#define MAX_SENSOR_VAL (1023)

#define NUM_SAMPLES (4)
#define SECONDS_8   (8000UL)

#define BLACK_LINE (0)
#define WHITE_LINE (1)

#define ON_LINE_THRESHOLD (800)
#define OFF_LINE_THRESHOLD (200)

#define NUM_SENSORS (8)

#define SETSPEED (175)      //arbitrary
#define ZEROREFTIME (1000)  //Delay before unit step kicks in
#define KPROP (100)         //Proportional controller constant
#define DIRECTIONPIN1 (7)
#define DIRECTIONPIN2 (10)

// motor input1 and input2 they should be PWM signals
#define SPEEDPIN1 (3)
#define SPEEDPIN2 (14)
#define MOTOR DELAY (500)
// motor output1 and 2   those pins are decided by pin configuration in Datasheet

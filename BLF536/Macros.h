#define PIN_ANALOG_0          (A0)      // Pin A0 for analog input - Left Sensor
#define PIN_ANALOG_1          (A1)      // Pin A1 for analog input - Right Sensor

#define PIN_S2                (A5)      // Pin A5 used for digital output - Mux select bit 2
#define PIN_S1                (A4)      // Pin A4 used for digital output - Mux select bit 1
#define PIN_S0                (A3)      // Pin A3 used for digital output - Mux select bit 0

#define PIN_LED_ARDUINO       (13)      // Arduino LED pin 13 for digital output

#define MULTIPLIER            (1000UL)  // General 1000 multiplier - used for various conversions

#define MIN_SENSOR_VAL        (0)       // Min sensor value = 0
#define MAX_SENSOR_VAL        (1023)    // Max sensor value = 1023

#define NUM_SAMPLES           (4)       // Number of samples to average
#define CALIBRATION_AMOUNT    (8000UL)  // Calibration limit

#define BLACK_LINE            (0)       // Black or white line selection
#define WHITE_LINE            (1)

#define ON_LINE_THRESHOLD     (800)     // On the line threshold
#define OFF_LINE_THRESHOLD    (200)     // Off the line threshold

#define NUM_SENSORS           (8)       // Number of sensors on the sensor array

#define SAMPLING_TIME         (10)      // Sampling time (T) in MILLISECONDS

#define BEGIN_TIME            (2000UL)            // Time to begin system impulse for system identification and to begin recording
#define DELTA_TIME            (BEGIN_TIME + 5);   // Complete the impulse for the system identification one T later
#define IMPULSE_VALUE         (1000UL)            //  The error to create for the system identification IN MILLIMETERS
#define MAX_NUM_SAMPLES       (500)               // Number of samples to record


// --- EVERYTHING BELOW THIS NEEDS TO BE UPDATED WITH COMMENTS --- //


#define SETSPEED              (175)               //arbitrary
#define ZEROREFTIME           (1000)              //Delay before unit step kicks in
#define KPROP                 (1)                 //Proportional controller constant
#define DIRECTIONPIN1         (7)
#define DIRECTIONPIN2         (10)

// Motor input1 and input2 they should be PWM signals
#define SPEEDPIN1 (3)
#define SPEEDPIN2 (14)
#define MOTOR DELAY (500)
// Motor output1 and 2   those pins are decided by pin configuration in Datasheet

#define STRAIGHTTIME 1000
#define RIGHTTURNTIME 1000
#define LEFTTURNTIME 1000

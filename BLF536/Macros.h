
// --- HARDWARE I/O PINS --- //
#define PIN_ANALOG_0          (A0)      // Pin A0 for analog input - Left Sensor
#define PIN_ANALOG_1          (A1)      // Pin A1 for analog input - Right Sensor

#define PIN_S2                (A5)      // Pin A5 used for digital output - Mux select bit 2
#define PIN_S1                (A4)      // Pin A4 used for digital output - Mux select bit 1
#define PIN_S0                (A3)      // Pin A3 used for digital output - Mux select bit 0

#define PIN_LED_ARDUINO       (13)      // Arduino LED pin 13 for digital output


// --- READING SENSORS --- //
#define MULTIPLIER            (1000L)   // General 1000 multiplier - used for various conversions

#define MIN_SENSOR_VAL        (0L)       // Min sensor value = 0
#define MAX_SENSOR_VAL        (1023L)    // Max sensor value = 1023

#define NUM_SAMPLES           (1L)       // Number of samples to average
#define CALIBRATION_AMOUNT    (8000L)    // Calibration limit

#define BLACK_LINE            (0)       // Black or white line selection
#define WHITE_LINE            (1)

#define ON_LINE_THRESHOLD     (800)     // On the line threshold
#define OFF_LINE_THRESHOLD    (200)     // Off the line threshold

#define NUM_SENSORS           (8)       // Number of sensors on the sensor array

#define SAMPLING_TIME         (10)      // Sampling time (T) in MILLISECONDS
#define SAMPLING_TIME_MICRO   (10000L)  // Sampling time (T) in MICROSECONDS

#define HOLD_AMOUNT           (5)       // Number of past calculations we are holding onto

/*
  
 --- TO BE DELETED ---
 
#define BEGIN_TIME            (0UL)               // Time to begin system impulse for system identification and to begin recording
#define DELTA_TIME            (BEGIN_TIME + 5);   // Complete the impulse for the system identification one T later
#define IMPULSE_VALUE         (1000L)             // The error to create for the system identification IN MILLIMETERS
#define MAX_NUM_SAMPLES       (500)               // Number of samples to record

*/

// --- MOTOR CONTROL --- ///
#define SETSPEED              (100L)     // Arbitrary - found through experemental values
#define KPROP                 (100L)     // Proportional controller constant
#define MAX_SPEED_VAL         (255L)     // Max input into the motors


// --- EVERYTHING BELOW THIS IS UNFINISHED --- //


#define STRAIGHTTIME          (1000)
#define RIGHTTURNTIME         (1000)
#define LEFTTURNTIME          (1000)

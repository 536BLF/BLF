#include "Macros.h"
//#include "TimerOne.h"

volatile int analog0[NUM_SENSORS];
volatile int analog1[NUM_SENSORS];

int oldAnalog0[NUM_SENSORS];
int oldAnalog1[NUM_SENSORS];

int sensorMin = MAX_SENSOR_VAL;        // minimum sensor value
int sensorMax = MIN_SENSOR_VAL;        // maximum sensor value

unsigned long lineVal0;
unsigned long lineVal1;

float error0;
float error1;
float totalError;

unsigned long timerVal;

enum {INIT, READ_2_LINE, READ_ANALOG0_LINE, READ_ANALOG1_LINE, READ_NO_LINE, INTERSECTION} state = INIT;

#define USE_INTERRUPTS (0)

#if USE_INTERRUPTS
void timerIsr(){
    ReadAllSensors(BLACK_LINE);
}
#endif

/*
 * Function: setup
 * Description: Setting the initial states and calibrating the sensors
 */
void setup() {
  state = INIT;                           // Initial state of the FSM
  Init_Sensors();                         // Initializing the sensors
  
  #if USE_INTERRUPTS
    Timer1.initialize(10000);             // set a timer of length 1000 microseconds (or 0.001 sec - or 1000Hz)
    Timer1.attachInterrupt( timerIsr );   // attach the service routine here
  #else
    Serial.begin(9600);
  #endif
}

/*
 * Function: loop
 * Description: The main the loop of the vehicle
 */
void loop() {
  int a;
  #if USE_INTERRUPTS
  
    if(analog0[4] >500) digitalWrite(13,HIGH);    // Flashing the LED if interrupts are used
    else digitalWrite(13, LOW);
    
  #else
  
    //Start_Timer();
    
    ReadAllSensors(BLACK_LINE);                   // Reads and records all the sensor values
    FSM();

    //Display_Timer();
    
    // --- Uncomment any of these below if you want to see specific outputs --- //
    
    /* 
    Sensor_Error_Calc(PIN_ANALOG_0);
    Serial.print("Analog0:\t"); Serial.print(error0); Serial.print("\t");
    Sensor_Error_Calc(PIN_ANALOG_1);
    Serial.print("Analog0:\t"); Serial.print(error1); Serial.print("\t");
    Total_Error_Calc(2);
    Serial.print("Total:\t"); Serial.println(totalError);
    */
    
<<<<<<< HEAD
    Print_Sensor_Values(PIN_ANALOG_0);
    Print_Sensor_Values(PIN_ANALOG_1);
=======
<<<<<<< HEAD
    Print_Sensor_Values();
=======
    //Print_Sensor_Values(PIN_ANALOG_0);
    //Print_Sensor_Values(PIN_ANALOG_1);
>>>>>>> f2fecf09747de95d9aad707769f2c41213b1a61f
>>>>>>> 4527f48a4ac592901504748ccdb9c0545f62f10f
    
    //lineVal0 = Get_Line_Value(PIN_ANALOG_0);
    //Serial.print("Analog0:\t"); Serial.print(lineVal0); Serial.print("\t");
    //lineVal1 = Get_Line_Value(PIN_ANALOG_1);
    //Serial.print("Analog1:\t"); Serial.println(lineVal1);
    
  #endif

}

/*
 * Function: Init_Sensors
 * Description: Initializing the pins that are used for the sensors.
 */
void Init_Sensors(void){
  pinMode(PIN_S2, OUTPUT);            // Select S2 - PIN A5
  pinMode(PIN_S1, OUTPUT);            // Select S1 - PIN A4
  pinMode(PIN_S0, OUTPUT);            // Select S0 - PIN A3
  pinMode(PIN_LED_ARDUINO, OUTPUT);   // Arduino LED
  Calibrate();
}

/*
 * Function: Mux_Select
 * Description: Selects the specific mux line depending on the select
 */
void Mux_Select(int select){

  switch(select){
    case 0: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, LOW);  break;    // 000
    case 1: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, HIGH);  break;   // 001
    case 2: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, LOW);  break;   // 010
    case 3: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, HIGH);  break;  // 011
    case 4: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, LOW);  break;   // 100
    case 5: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, HIGH);  break;  // 101
    case 6: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, LOW);  break;  // 110
    case 7: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, HIGH);  break; // 111
    default: return;
  }

  delayMicroseconds(4);   // The delay necessary for the mux to switch
}


/*
 * Function: Total_Error_Calc
 * Description: Gives the total error depending on the input
 */
void Total_Error_Calc(int select){
  switch(select){
    case 0:
      totalError = error0;                // When the line for sensor 1 has disappeared

    case 1:
      totalError = error1;                // When the line for sensor 0 has disappeared

    case 2:
      totalError = ((-error0) + error1)/2;   // Average of the sensor errors
  }
}


/*
 * Function: Sensor_Error_Calc
 * Description: Calculates the sensor error  in CENTIMENTERS for either sensor 0 or sensor 1
 */
void Sensor_Error_Calc(int select){
  
   switch(select){
    case PIN_ANALOG_0:
    
      lineVal0 = Get_Line_Value(PIN_ANALOG_0);                        // Get the line value between 0 - 7000
      error0 = ((float)lineVal0 - (float)ZERO_ERROR_0) / (float)1000;   // Get the error of the sensor in cm
      break;

    case PIN_ANALOG_1:

      lineVal1 = Get_Line_Value(PIN_ANALOG_1);
      error1 = ((float)lineVal1 - (float)ZERO_ERROR_1 ) / (float)1000;
      break;

    default: break;
   }

   return;
}


/*
 * Function: ReadAllSensors
 * Description: Obtains the values for the sensors for both sensor 0 and sensor 1 and maps the values between 0 - 1023
 */
void ReadAllSensors(int invert){
  int tmp0, tmp1, i;

  Array_Copy(PIN_ANALOG_0);                                   // Copying the current values to the old values to have something to hold onto.
  Array_Copy(PIN_ANALOG_1);
  
  for(i=0 ; i<NUM_SENSORS ; i++){
    
    Mux_Select(i);                                            // Cycling through the mux
    
    tmp0 = Read_Average(PIN_ANALOG_0, NUM_SAMPLES);           // Obtaining an average of the readings
    tmp1 = Read_Average(PIN_ANALOG_1, NUM_SAMPLES);

    tmp0 = map(tmp0, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Obtaining the sensor values between the calibrated min and max and mapping them between 0 - 1023
    tmp1 = map(tmp1, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    tmp0 = constrain(tmp0, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Constraining the values between 0 - 1023
    tmp1 = constrain(tmp1, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    if(invert){
      tmp0 = MAX_SENSOR_VAL - tmp0;                           // If we are reading a white line, then the values will invert
      tmp1 = MAX_SENSOR_VAL - tmp1;                           // NO MATTER WHAT: ~0 -> Off the line    ,     ~1023 -> On the line
    }
    
    analog0[i] = tmp0; 
    analog1[i] = tmp1;

  }
}


/*
 * Function: Calibrate
 * Description: Running through the calibration steps of the sensors
 */
void Calibrate(void){
  int i, sensorVal0, sensorVal1;

  digitalWrite(PIN_LED_ARDUINO, HIGH);        // Arduino LED ON during calibration
  
  while (millis() < SECONDS_8) {              // calibrate during the first 8 seconds
    
    i++;
    Mux_Select(floor(i/MULTIPLIER));          // Cycling through the mux every 1 second

    sensorVal0 = analogRead(PIN_ANALOG_0);    // Reading the sensor
    sensorVal1 = analogRead(PIN_ANALOG_1);

    if (sensorVal0 > sensorMax) sensorMax = sensorVal0;   // record the maximum sensor value
    if (sensorVal1 > sensorMax) sensorMax = sensorVal1;

    if (sensorVal0 < sensorMin) sensorMin = sensorVal0;   // record the minimum sensor value
    if (sensorVal1 < sensorMin) sensorMin = sensorVal1;
  }

  digitalWrite(PIN_LED_ARDUINO, LOW);
  
}


/*
 * Function: Read_Average
 * Description: Obtains an average of the readings when reading in from the sensors
 */
int Read_Average(int select, int num_samples){
  int i;
<<<<<<< HEAD
  int total =0;
=======
  int total = 0;
>>>>>>> f2fecf09747de95d9aad707769f2c41213b1a61f

  switch(select){
    case PIN_ANALOG_0: 
      
      for(i=0 ; i<num_samples ; i++){
        total += analogRead(PIN_ANALOG_0);    // Total value from all the samples
      }
      break;
            
    case PIN_ANALOG_1: 
    
      for(i=0 ; i<num_samples ; i++){
        total += analogRead(PIN_ANALOG_1);
      }
      break;

    default: return 0;
  }

  return (total/NUM_SAMPLES);                 // Returnin the average from the number of samples
}


/*
 * Function: Get_Line_Value
 * Description: Obtaining the value between 0 - 7000 where the center of the line is located according to the sensors
 * 
 * Equation Used:     (Sensor0 * 0) + (Sensor1 * 1000) + (Sensor2 * 2000) + (Sensor3 * 3000) ...
 *                    --------------------------------------------------------------------------
 *                    (   Sensor0   +       Sensor1     +     Sensor2     +     Sensor3 ...
 *                    
 * Exact Center Value = 3500
 * 
 */
unsigned long Get_Line_Value(int select){
 int i;
 unsigned long total = 0;
 unsigned long sum = 0;
 unsigned long out = 0;

 switch(select){
  case PIN_ANALOG_0: 
  
    for(i=0 ; i<NUM_SENSORS ; i++){
      total += analog0[i] * (i * MULTIPLIER);   // Numerator of Equation
      sum += analog0[i];                        // Denominator of Equation
    }
    break;
      
  case PIN_ANALOG_1: 
  
    for(i=0 ; i<NUM_SENSORS ; i++){
      total += analog1[i] * (i * MULTIPLIER);
      sum += analog1[i];
    }
    break;
      
  default: return out;
 }

 out = total/sum;                               // Dividing numerator by denominator
 return out;
}


// --- TESTING CODE BELOW ---


/*
 * Function: Print_Sensor_Values
 * Description: Prints the sensor values of analog0
 */
void Print_Sensor_Values(int select){
  int i;
  
  switch(select){
    case PIN_ANALOG_0:
      
      Serial.print("ANALOG 0 SENSORS:");
      for(i=0 ; i<NUM_SENSORS ; i++){
        Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(analog0[i]);
      }
      Serial.println("");
      break;
    
    case PIN_ANALOG_1:
      
      Serial.print("ANALOG 1 SENSORS");
      for(i=0 ; i<NUM_SENSORS ; i++){
        Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(analog1[i]);
      }
      Serial.println("");
      break;
    
    default: break;
  }
}

/*
 * Function: Start_Timer
 * Description: Starts a local timer
 */
void Start_Timer(void){
  timerVal = micros();
}

/*
 * Function: Display Timer
 * Description: Displays the amount of time in MICROSECONDS that has gone by since the timer was started
 */
void Display_Timer(void){
  Serial.println(micros()-timerVal);
}


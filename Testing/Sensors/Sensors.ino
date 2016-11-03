#include "Macros.h"

int analog0[8];
int analog1[8];

int sensorMin = MAX_SENSOR_VAL;        // minimum sensor value
int sensorMax = MIN_SENSOR_VAL;        // maximum sensor value

unsigned long lineVal0;
unsigned long lineVal1;

void setup() {
  Serial.begin(9600);
  Init_Sensors();

}

void loop() {
  
  ReadAllSensors(BLACK_LINE);
  lineVal0 = Get_Line_Value(PIN_ANALOG_0);
  Serial.println(lineVal0);

}

void Init_Sensors(void){
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_LED_0, OUTPUT);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_ARDUINO, OUTPUT);
  digitalWrite(PIN_LED_0, HIGH);
  digitalWrite(PIN_LED_1, HIGH);
  Calibrate();
}

void Mux_Select(int sel){

  switch(sel){
    case 0: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, LOW);  break;
    case 1: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, HIGH);  break;
    case 2: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, LOW);  break;
    case 3: digitalWrite(PIN_S2, LOW); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, HIGH);  break;
    case 4: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, LOW);  break;
    case 5: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, LOW); digitalWrite(PIN_S0, HIGH);  break;
    case 6: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, LOW);  break;
    case 7: digitalWrite(PIN_S2, HIGH); digitalWrite(PIN_S1, HIGH); digitalWrite(PIN_S0, HIGH);  break;;
    default: return;
  }

 
}

void ReadAllSensors(int invert){
  int tmp0, tmp1, i;
  
  for(i=0;i<8;i++){
    
    Mux_Select(i);
    
    tmp0 = Read_Average(PIN_ANALOG_0, NUM_SAMPLES);
    tmp1 = Read_Average(PIN_ANALOG_1, NUM_SAMPLES);

    tmp0 = map(tmp0, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);
    tmp1 = map(tmp1, sensorMin, sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    tmp0 = constrain(tmp0, MIN_SENSOR_VAL, MAX_SENSOR_VAL);
    tmp1 = constrain(tmp1, MIN_SENSOR_VAL, MAX_SENSOR_VAL);

    if(invert){
      tmp0 = MAX_SENSOR_VAL - tmp0;
      tmp1 = MAX_SENSOR_VAL - tmp1;
    }
    
    analog0[i] = tmp0; 
    analog1[i] = tmp1;

  }

}


void Calibrate(void){
  int i, sensorVal0, sensorVal1;

  digitalWrite(PIN_LED_ARDUINO, HIGH);
  
  // calibrate during the first 8 seconds
  while (millis() < SECONDS_8) {
    
    i++;
    Mux_Select(floor(i/MULTIPLIER));

    sensorVal0 = analogRead(PIN_ANALOG_0);
    sensorVal1 = analogRead(PIN_ANALOG_1);

    // record the maximum sensor value
    if (sensorVal0 > sensorMax) sensorMax = sensorVal0;
    if (sensorVal1 > sensorMax) sensorMax = sensorVal1;

    // record the minimum sensor value
    if (sensorVal0 < sensorMin) sensorMin = sensorVal0;
    if (sensorVal1 < sensorMin) sensorMin = sensorVal1;
  }

  digitalWrite(PIN_LED_ARDUINO, LOW);
  
}


int Read_Average(int select, int num_samples){
  int i, total;

  switch(select){
    case PIN_ANALOG_0: 
      
      for(i=0 ; i<num_samples ; i++){
        total += analogRead(PIN_ANALOG_0);
      }
      break;
            
    case PIN_ANALOG_1: 
    
      for(i=0 ; i<num_samples ; i++){
        total += analogRead(PIN_ANALOG_1);
      }
      break;

    default: return 0;
  }

  return (total/num_samples);
}


unsigned long Get_Line_Value(int select){
 int i;
 unsigned long total, sum, out;

 switch(select){
  case PIN_ANALOG_0: 
  
    for(i=0 ; i<8 ; i++){
      total += analog0[i] * (i * MULTIPLIER);
      sum += analog0[i];
    }
    break;
      
  case PIN_ANALOG_1: 
  
    for(i=0 ; i<8 ; i++){
      total += analog1[i] * (i * MULTIPLIER);
      sum += analog1[i];
    }
    break;
      
  default: return out;
 }

 out = total/sum;
 return out;
}


void Print_Sensor_Values(void){
  int i;
  for(i=0 ; i<8 ; i++){
    Serial.print(",\t"); Serial.print(i); Serial.print(":"); Serial.print(analog0[i]);
  }
  Serial.println("");
}


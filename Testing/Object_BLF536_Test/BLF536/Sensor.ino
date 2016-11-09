Sensor::Sensor(const uint8_t& pinVal, int whiteLine) {
  this->pin = pinVal;
  this->whiteLine = whiteLine;
  
  this->sensorMin = MAX_SENSOR_VAL;
  this->sensorMax = MIN_SENSOR_VAL;
}

void Sensor::Mux_Select(int muxSelect) {

  switch (muxSelect) {
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


void Sensor::Init_Sensors(void) {

  pinMode(PIN_S2, OUTPUT);            // Select S2 - PIN A5
  pinMode(PIN_S1, OUTPUT);            // Select S1 - PIN A4
  pinMode(PIN_S0, OUTPUT);            // Select S0 - PIN A3
  pinMode(PIN_LED_ARDUINO, OUTPUT);   // Arduino LED

  this->Calibrate();
}

void Sensor::Calibrate(void) {
  int i = 0;
  int sensorVal = 0;

  digitalWrite(PIN_LED_ARDUINO, HIGH);        // Arduino LED ON during calibration

  while (i < SECONDS_8) {                     // calibrate during the first 8 seconds

    i++;
    this->Mux_Select(floor(i / MULTIPLIER));    // Cycling through the mux every 1 second

    sensorVal = analogRead(this->pin);    // Reading the sensor

    if (sensorVal > this->sensorMax) this->sensorMax = sensorVal;   // record the maximum sensor value
    if (sensorVal < this->sensorMin) this->sensorMin = sensorVal;   // record the minimum sensor value
    
  }

  this->ReadAllSensors();

  this->Get_Line_Value();
  this->initLineVal = this->lineVal;

  digitalWrite(PIN_LED_ARDUINO, LOW);
}


void Sensor::ReadAllSensors(void) {
  int i = 0;
  this->tmp = 0;
  
  this->Array_Copy();                                   // Copying the current values to the old values to have something to hold onto.

  for (i = 0 ; i < NUM_SENSORS ; i++) {

    this->Mux_Select(i);                                            // Cycling through the mux

    this->tmp = this->Read_Average();     // Obtaining an average of the readings
    //Serial.print(this->pin);  Serial.print(" ");  Serial.println(tmp);

    this->tmp = map(this->tmp, this->sensorMin, this->sensorMax, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Obtaining the sensor values between the calibrated min and max and mapping them between 0 - 1023

    this->tmp = constrain(this->tmp, MIN_SENSOR_VAL, MAX_SENSOR_VAL);   // Constraining the values between 0 - 1023

    if (this->whiteLine) {
      this->tmp = MAX_SENSOR_VAL - this->tmp;                           // If we are reading a white line, then the values will invert
    }

    this->sensorVals_T0[i] = this->tmp;

  }
}


void Sensor::Array_Copy(void) {
  int i;
  for (i = 0 ; i < NUM_SENSORS ; i++) {
    this->sensorVals_T2[i] = this->sensorVals_T1[i];
    this->sensorVals_T1[i] = this->sensorVals_T0[i];
  }
}


int Sensor::Read_Average(void) {
  int i;
  int total = 0;

  for (i = 0 ; i < NUM_SAMPLES ; i++) {
    total += analogRead(this->pin);    // Total value from all the samples
  }

  return (total / NUM_SAMPLES);               // Returnin the average from the number of samples
}


void Sensor::Get_Line_Value(void) {
  int i;
  unsigned long total = 0;
  unsigned long sum = 0;
  unsigned long out = 0;

  for (i = 0 ; i < NUM_SENSORS ; i++) {
    total += this->sensorVals_T0[i] * (i * MULTIPLIER);   // Numerator of Equation
    sum += this->sensorVals_T0[i];                        // Denominator of Equation
  }

  out = total / sum;                          // Dividing numerator by denominator
  this->lineVal = out;

}


void Sensor::Sensor_Error_Calc(void) {

  Get_Line_Value();                        // Get the line value between 0 - 7000
  this->error = ((float)this->lineVal - (float)this->initLineVal) / (float)1000UL;   // Get the error of the sensor in cm

}



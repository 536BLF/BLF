//#include "Sensor.ino"

class Sensor {
  public:
    //variables
    int whiteLine;
    int sensorVals_T0[NUM_SENSORS];
    int sensorVals_T1[NUM_SENSORS];
    int sensorVals_T2[NUM_SENSORS];
    int sensorMin;                  // minimum sensor value
    int sensorMax;                  // maximum sensor value
    int tmp;
    uint8_t pin;
    float error;
    float totalError;
    unsigned long lineVal;
    unsigned long initLineVal;
    unsigned long timerVal;


    // Functions

    void Init_Sensors();
    void ReadAllSensors(void);
    void Get_Line_Value(void);
    void Sensor_Error_Calc(void);
    void Calibrate(void);
    void Mux_Select(int);
    void Array_Copy(void);
    int Read_Average(void);
    void Hold_Value(void);

    // Constructor
    Sensor(const uint8_t&, int);

};

//#include "Tester.ino"


class Tester {
  public:
    unsigned long timerVal;
    unsigned long sample = 0;

    void Print_Sensor_Values(void);
    void Print_Line_Values(void);
    void Print_Total_Error(void);
    void Start_Timer(void);
    void Display_Timer(void);
    void System_Id(void);

    Tester();
};

/***************************************************
  This is a declaration of class CarbonDioxideSensor for working with MH-Z19 sensor with Adafruit 0.56 inch 7 segment indicator
 ****************************************************/
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SoftwareSerial.h>

#ifndef CARBON_DIOXIDE_SENSOR_H_
#define CARBON_DIOXIDE_SENSOR_H_

namespace MhZ19Sensor
{
    
struct SensorData
{
  int temperature; // in Celcius degrees
  unsigned int carbon; // in ppm 
  int delta_carbon; // in ppm
};

enum SensorType
{
  TEMP,
  CARBON,
  DELTA_CARBON,
  MAX
};

class CarbonDioxideSensor
{
public:
    // constructor
    CarbonDioxideSensor(const int txPin, const int rxPin, bool enableDebug = false);
    
    void pauseSensorMs(unsigned int delay);
    
    void showResult(Adafruit_7segment& matrix, SensorType sensorType);

    void setup();
//private:
    
    SensorData getSensorData();

private:
    static const int DELAY; // msec
    
    // number of average groups to average on
    // if N_AVERAGE == 60 (5 minute) and QUEUE_SIZE == 12, then period for DELTA is 1 hour = 12 * 5
    // if N_AVERAGE == 60 (5 minute) and QUEUE_SIZE == 36, then period for DELTA is 3 hours
    
    //static const int QUEUE_SIZE = 12; 
    
    // number of values to average on and store into the queue, one minute average value, will be goten every 5sec
    // 12 - 1 minute
    // 60 - 5 minute
    //static const int N_AVERAGE; // msec
    
    //SensorData m_queue[QUEUE_SIZE]; // cyclic queue
    //bool m_isInit;
    //unsigned int m_iQueue;
    //unsigned int m_iAverage;
    bool m_isSensorInitialized;
    //bool isQueueInitialized;
    unsigned int m_lastUpdateTime;

    SoftwareSerial m_sensorSerial;
};

} // end of Sensor namespace

#endif // CARBON_DIOXIDE_SENSOR_H_
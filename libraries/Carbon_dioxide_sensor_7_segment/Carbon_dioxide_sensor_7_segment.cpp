/***************************************************
  This is an implementation of class CarbonDioxideSensor for working with MH-Z19 sensor with Adafruit 0.56 inch 7 segment indicator
 ****************************************************/
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include "Carbon_dioxide_sensor_7_segment.h"

namespace
{
    const byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
    const int DEFAULT_CARBON_PPM = 410;
    const int START_CARBON_PPM = 400;
    void(* resetFunc) (void) = 0;
}; // end of anonymous namespace

namespace MhZ19Sensor
{
//const int CarbonDioxideSensor::DELAY = 10000; // msec
//const int CarbonDioxideSensor::N_AVERAGE = 60; 

CarbonDioxideSensor::CarbonDioxideSensor(const int txPin, 
                                         const int rxPin, 
                                         bool enableDebug)
    // : m_isInit (false)
    //, isQueueInitialized(false)
    : m_sensorSerial(txPin, rxPin)
    , m_lastUpdateTime(millis())
{
    // init array of sensor values
    /*
    for (int i = 0; i < QUEUE_SIZE; ++i)
    {
        SensorData sensorData = {0, DEFAULT_CARBON_PPM, 0};
        m_queue[i] = sensorData;
    }
    */
}


void CarbonDioxideSensor::setup()
{
    m_sensorSerial.begin(9600);
}


void CarbonDioxideSensor::pauseSensorMs(unsigned int delay)
{
    while (millis() - m_lastUpdateTime < delay)
    {
        continue;
    }
    
    m_lastUpdateTime = millis();
    Serial.print(m_lastUpdateTime);
    Serial.print(" ");
}


void CarbonDioxideSensor::showResult(Adafruit_7segment& matrix, 
                                     SensorType sensorType)
{
    SensorData sensorData = getSensorData();
    if (0 == sensorData.carbon)
    {
        resetFunc();
    }        
    
    matrix.print(sensorData.carbon, DEC);
    matrix.writeDisplay();
    
    Serial.print(sensorData.carbon);
    Serial.println(" ppm CO2");
}

SensorData CarbonDioxideSensor::getSensorData()
{
    SensorData result;
    unsigned char response[9];
    
    m_sensorSerial.write(cmd, 9);
    memset(response, 0, 9);
    m_sensorSerial.readBytes(response, 9);
    int i;
    byte crc = 0;
    for (i = 1; i < 8; i++) 
    {
        crc+=response[i];
    }
    
    crc = 255 - crc;
    crc++;
    unsigned int ppm;
    
    if (!(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc)) 
    {
        ppm = 0;
        Serial.println("CRC error: " + String(crc) + " / " + String(response[8]));
    } 
    else 
    {
        unsigned int responseHigh = (unsigned int) response[2];
        unsigned int responseLow = (unsigned int) response[3];
        ppm = (256*responseHigh) + responseLow;
    }
    
    result.carbon = ppm;
    
    return result;
}

} // end of Sensor namespace

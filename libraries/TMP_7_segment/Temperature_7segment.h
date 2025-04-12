/*************************************************** 
  This is a library for using TMP sensor from standard Arduino Kit with Adafruit 0.56 inch 7 segment indicator

 ****************************************************/
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#ifndef TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SENSOR_H_

class TemperatureSensor
{
public:
    // constructor
	TemperatureSensor(const int sensorPin, const bool enableDebug = false);
	
	void proceedResult(Adafruit_7segment& matrix);
private:
    void proceedOneInteration(Adafruit_7segment& matrix);
	// @brief show float correctly on the 7 segment indicator
	void showTemperatureOn7segment(float temperature, Adafruit_7segment& matrix);
	float roundTemperatureUpToHalfDegree(float temperature);
	float convertAdcToTemperature(int sensorAdcValue);
	int getMedian();
	void putValueIntoSensorValues(int value);
	
private:
	static const int SIZE = 10;
	const int SENSOR_PIN;
	int sensorValues[SIZE];
	bool isDebugEnabled;
};

#endif // TEMPERATURE_SENSOR_H_
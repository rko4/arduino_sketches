#include <stdlib.h>
#include <math.h>

#include "Temperature_7segment.h"

namespace
{
	const int DEFAULT_INT = -1;
}; // end of anonyous namespace


int comp (const void * elem1, const void * elem2) 
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

TemperatureSensor::TemperatureSensor(const int sensorPin, const bool enableDebug)
	: SENSOR_PIN(sensorPin)
	, isDebugEnabled(enableDebug)
{
  // init sensorValues
  for (int i = 0; i < SIZE;  ++i)
  {
      sensorValues[i] = DEFAULT_INT;
  }
}

void TemperatureSensor::proceedResult(Adafruit_7segment& matrix)
{
  for (int i = 0; i < SIZE; ++i)
  {
	proceedOneInteration(matrix);
	delay(1000);        // delay in between reads for stability 
  }
}


void TemperatureSensor::proceedOneInteration(Adafruit_7segment& matrix)
{
  int rawSensorAdcValue = analogRead(SENSOR_PIN);
  putValueIntoSensorValues(rawSensorAdcValue);
  int medianSensorAdcValue = getMedian();
  float temperature = convertAdcToTemperature(medianSensorAdcValue);
  float roundTemperature = roundTemperatureUpToHalfDegree(temperature);
  //debug

//  Serial.print("Round temperature: ");
//  Serial.println(roundTemperature);
//  Serial.println();
  showTemperatureOn7segment(roundTemperature, matrix);
}

void TemperatureSensor::showTemperatureOn7segment(float temperature, Adafruit_7segment& matrix)
{
  int intTemperature = int(temperature*10);
  //matrix.print(intTemperature, DEC);
  matrix.writeDigitNum(1, intTemperature/100);
  matrix.writeDigitNum(3, (intTemperature/10)%10, true);
  matrix.writeDigitNum(4, intTemperature%10);
  matrix.writeDisplay();
}

float TemperatureSensor::roundTemperatureUpToHalfDegree(float temperature)
{
  return (round(temperature*2)/2.0);
}

float TemperatureSensor::convertAdcToTemperature(int sensorAdcValue)
{
  float voltage = 5.0 * sensorAdcValue / 1024.0;
  float temperature = (voltage - 0.5)*100;
  return temperature;
}

int TemperatureSensor::getMedian()
{
  int result = sensorValues[0];
  
  // condition of sensorValues is filled with values
  if (sensorValues[SIZE/2+1] != DEFAULT_INT)
  {
    int arrayToSort[SIZE];
    // copy values to sort them
    for(int i = 0; i < SIZE; ++i)
    {
      arrayToSort[i] = sensorValues[i];
    }
    
    qsort(arrayToSort, SIZE, sizeof(int), comp);
    //debug
//    Serial.print("arraySort: ");
//    for(int i = 0; i < SIZE; ++i)
//    {
//      Serial.print(arrayToSort[i]);
//      Serial.print(" ");
//    }
//    result = arrayToSort[SIZE/2-1];
//    Serial.println();
//    Serial.print("Median: ");
//    Serial.println(result);
  }
  
  return result;
}


void TemperatureSensor::putValueIntoSensorValues(int value)
{
  static int positionToOverWrite = 0;
  // debug
//  Serial.print("New value: ");
//  Serial.println(value);
//  Serial.print("Position:");
//  Serial.println(positionToOverWrite);
  
  // filling the array of input values
  sensorValues[positionToOverWrite] = value;
  ++positionToOverWrite;
  
  if (positionToOverWrite == SIZE)
  {
    positionToOverWrite = 0;
  }
  
  // debug
  // if (positionToOverWrite%10==0)
  // {
    // Serial.print("sensorValues: ");
    // for(int i = 0; i < SIZE; ++i)
    // {
      // Serial.print(sensorValues[i]);
      // Serial.print(" ");
    // }
    // Serial.println();
  // }
}
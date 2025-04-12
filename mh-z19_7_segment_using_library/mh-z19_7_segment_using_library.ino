/*************************************************** 
7 segments
C to A5
D to A4
+ to 5V
- to GND

MH Z19
A0 to TX of Sensor
A1 to RX of Sensor
Vin to 5V
GND to GND
 ****************************************************/
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include "Carbon_dioxide_sensor_7_segment.h"

using namespace MhZ19Sensor;

CarbonDioxideSensor carbonDioxideSensor (A0, A1); // A0 - to TX сенсора, A1 - to RX
Adafruit_7segment matrix = Adafruit_7segment();

void setup() 
{
  Serial.begin(9600);
  Serial.println("MH Z19 on 7segments is started");
  carbonDioxideSensor.setup();
  matrix.begin(0x70);
  matrix.write(0x76); 
  carbonDioxideSensor.pauseSensorMs(5000);
  carbonDioxideSensor.showResult(matrix, CARBON);
}

void loop()
{
  // model
  SensorData sensorData = carbonDioxideSensor.getSensorData();
  // view
  carbonDioxideSensor.showResult(matrix, CARBON);
  carbonDioxideSensor.pauseSensorMs(5000);
}

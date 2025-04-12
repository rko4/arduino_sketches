;/*************************************************** 
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
#include <SoftwareSerial.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_SSD1306.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

#define ENABLE_DEBUG
#define ENABLE_QUEUE_DEBUG
#define ABS(x)  ((x<0)?(-x):(x))

struct sensor_t
{
  int temperature; // in Celcius degrees
  unsigned long carbon; // in ppm 
  long delta_carbon; // in ppm
};

enum sensor_type_t
{
  TEMP,
  CARBON,
  DELTA_CARBON,
  MAX
};

const unsigned int DELAY = 6000; // ms
const unsigned int DEFAULT_CARBON_PPM = 410;
const unsigned int START_CARBON_PPM = 400;
const unsigned int QUEUE_SIZE = 12;  // number of average groups to average on
                            // if N_AVERAGE == 60 (5 minute) and QUEUE_SIZE == 12, then period for DELTA is 1 hour = 12 * 5
                            // if N_AVERAGE == 60 (5 minute) and QUEUE_SIZE == 36, then period for DELTA is 3 hours
const unsigned int N_AVERAGE = 60; // number of values to average on and store into the queue, one minute average value, will be goten every 5sec
                          // 12 - 1 minute
                          // 60 - 5 minute
const byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 

sensor_t queue[QUEUE_SIZE]; // cyclic queue
unsigned char response[9]; // digital response from sensor
bool isQueueInitialized;
bool isInit;

SoftwareSerial mySerial(A0, A1); // A0 - к TX сенсора, A1 - к RX
Adafruit_7segment matrix = Adafruit_7segment();
//Adafruit_SSD1306 display(-1);

void(* resetFunc) (void) = 0;

void setup() 
{
#ifdef ENABLE_DEBUG
  Serial.begin(9600);
  Serial.println("MH Z19 on 7segments is started");
#endif
  mySerial.begin(9600);
  matrix.begin(0x70);
  matrix.write(0x76); 

  isQueueInitialized = false;
  isInit = false;

  sensor_t sensorData;
  sensorData.temperature = 0;
  sensorData.carbon = DEFAULT_CARBON_PPM;
  sensorData.delta_carbon = 0;

  // init array of sensor values
  for (int i = 0; i < QUEUE_SIZE; ++i)
  {
    queue[i] = sensorData;
  }
}


void loop()
{
  sensor_t sensor = {0, 0, 0};
  
  static sensor_type_t sensorType = TEMP;
  
  // initialization
  if (isInit)
  {
    sensor = getCarbonOrTemperature(isInit);
    if (0 == sensor.carbon)
    {
      resetFunc();
    }
  }
  else
  {
    sensor = getCarbonOrTemperature(isInit);
    isInit = true;
  }
  
  // swap between CO2 and temperature and delta
  switch (sensorType)
  {
    case TEMP:
      //matrix.print(sensor.temperature, DEC);
      // print C sign
      matrix.clear();
      
      matrix.writeDigitNum(0, sensor.temperature/10);
      matrix.writeDigitNum(1, sensor.temperature%10);
    
      matrix.writeDigitRaw(4,0x39); // C
      break;
    case CARBON:
      matrix.print(sensor.carbon, DEC);
      break;
    case DELTA_CARBON:
    {
      unsigned int counter = ABS(sensor.delta_carbon);
#ifdef ENABLE_DEBUG
      Serial.print(counter);
      Serial.println(" Delta ppm CO2 - print");
#endif
      matrix.clear();
      if (sensor.delta_carbon > 0)
      {
        matrix.writeDigitRaw(0,0x23); // UP  
      }
      else if (sensor.delta_carbon < 0)
      {
        matrix.writeDigitRaw(0,0x1C); // DOWN
      }
      
      char firstDigit = (counter / 100) % 10;
      // if (firstDigit > 0) matrix.writeDigitNum(1, firstDigit);// do not print leading zeros

      // print leading zeros
      matrix.writeDigitNum(1, firstDigit);
      
      // char secondDigit = (counter / 100) % 10;
      //if (firstDigit > 0 || secondDigit > 0) matrix.writeDigitNum(2, secondDigit); // do not print leading zeros

      // print leading zeros
      //matrix.writeDigitNum(1, secondDigit); 

      matrix.drawColon(false);

      char thirdDigit = (counter / 10) % 10;
      // if (firstDigit > 0 || secondDigit > 0 || thirdDigit > 0) matrix.writeDigitNum(3, thirdDigit); // do not print leading zeros
      // print leading zeros
      matrix.writeDigitNum(3, thirdDigit); 
      
      matrix.writeDigitNum(4, counter % 10);
      //matrix.print(sensor.delta_carbon, DEC);
      break;
    }
    default:
      break;
  }
  /*
  if (TEMP == sensorType)
  {
    matrix.print(sensor.temperature, DEC);
  }
  else if (CARBON == sensorType)
  {
    matrix.print(sensor.carbon, DEC);
  }
  else if (DELTA_CARBON == sensorType)
  {
    matrix.print(sensor.delta_carbon, DEC);
  }
  */
  matrix.writeDisplay();
  /* sw
  // switch to another sensor data CARBON <-> DELTA_CARBON
  if (sensorType == CARBON)
  {
    sensorType = DELTA_CARBON;
  }
  else
  {
    sensorType = CARBON;
  }
  */
  // switch to another sensor data TEMP -> CARBON -> DELTA_CARBON ->TEMP
  if (sensorType == TEMP)
  {
    sensorType = CARBON;
  }
  else if (sensorType == CARBON)
  {
    sensorType = DELTA_CARBON;
  }
  else
  {
    sensorType = TEMP;
  }
  
  
  //Serial.print(ppm);
  //Serial.println(" ppm CO2");
}


sensor_t getCarbonOrTemperature(bool makeDelay)
{
  //int i;
  
  sensor_t result;
  result.temperature = 0;
  result.carbon = DEFAULT_CARBON_PPM;
  result.delta_carbon = 0;
  
  static unsigned int iQueue = 0;
  static unsigned long iAverage = 0;
  static bool isSensorInitialized = false;
  
  if (!makeDelay)
  {
    delay(DELAY);
  }
  
  mySerial.write(cmd, 9);
  memset(response, 0, 9);
  mySerial.readBytes(response, 9);
  
  byte crc = 0;
  for (int i = 1; i < 8; i++) 
  {
    crc+=response[i];
  }
  crc = 255 - crc;
  crc++;

  
  if (!(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc)) 
  {
    Serial.println("CRC error: " + String(crc) + " / " + String(response[8]));
  } 
  else // if response is correct
  {
    const unsigned int responseHigh = (unsigned int) response[2];
    const unsigned int responseLow = (unsigned int) response[3];
    const int temperature = (int)response[4] - 42;
    const unsigned int ppm = (256*responseHigh) + responseLow;
    
    result.carbon = ppm;
    result.temperature = temperature;
    result.delta_carbon = 0;

#ifdef ENABLE_DEBUG
      Serial.print(result.carbon);
      Serial.println(" ppm CO2 - read from sensor");
#endif

    // get the difference
    if (isSensorInitialized ||
      (ppm != DEFAULT_CARBON_PPM && ppm != START_CARBON_PPM))
    {
      isSensorInitialized = true;
      // storage of average values
      if (iQueue < QUEUE_SIZE)
      {
        int delta_carbon;
        // initialize queue to calculate CARBON DELTA
        if (!isQueueInitialized && iQueue == 0 && 0 == iAverage)
        {
          queue[0].carbon = ppm;
        }
        
        // get delta carbon
        if (isQueueInitialized)
        {
          delta_carbon = static_cast<int>(ppm) - queue[iQueue].carbon;
        }
        else
        {
          delta_carbon = static_cast<int>(ppm) - queue[0].carbon;
        }

        result.delta_carbon = delta_carbon;

        // averaging
        if (iAverage == 0)
        {
          result.carbon = ppm;
          ++iAverage;
        }
        else if (iAverage > 0 && iAverage < N_AVERAGE)
        {
          result.carbon = (result.carbon * iAverage + ppm)/(iAverage + 1);
          ++iAverage;
        }
        else // iAverage == N_AVERAGE
        {
          queue[iQueue] = result;
          iAverage = 0;
          ++iQueue;
        }

#ifdef ENABLE_DEBUG

#ifdef ENABLE_QUEUE_DEBUG
        int i;
        Serial.print("Carbon Queue: ");
        for (i = 0; i < QUEUE_SIZE; ++i)
        {
          Serial.print(queue[i].carbon);
          Serial.print(" ");
        }
        Serial.println(" ");
#endif // ENABLE_QUEUE_DEBUG

        Serial.print("iAverage = ");
        Serial.print(iAverage);
        
        Serial.print("; iQueue = ");
        Serial.print(iQueue);

        Serial.print("; Measured ppm CO2 = ");
        Serial.print(ppm);

        Serial.print("; Average ppm CO2 = ");
        Serial.print(result.carbon);

        if (isQueueInitialized)
        {
          Serial.print("; Average ppm CO2 for DELTA to subtract = ");
          Serial.print(queue[iQueue].carbon);
        }
        else
        {
          Serial.print("; Average ppm CO2 for DELTA to subtract = ");
          Serial.print(queue[0].carbon);
        }

        Serial.print("; DELTA ppm CO2 = ");
        Serial.println(result.delta_carbon);
        Serial.println("");
#endif // ENABLE_DEBUG

      } // if (iQueue < QUEUE_SIZE - 1)
      else
      {
        iQueue = 0;
        isQueueInitialized = true;
        Serial.println("       !!!!!!    QUEUE IS INITIALIZED !!!!!!");
      }
    } // if (ppm != DEFAULT_CARBON_PPM)
  } // else // if response is correct
  
  if (makeDelay)
  {
    delay(DELAY);
  }
  
  return result;
}

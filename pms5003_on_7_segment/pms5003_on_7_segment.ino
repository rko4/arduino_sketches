// On Leonardo/Micro or others with hardware serial, use those!
// uncomment this line:
// #define pmsSerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_SSD1306.h>

// OLED display TWI address
#define OLED_ADDR   0x3C
#define MAX(x, y) ((x) > (y) ? x : y)

//bool readPMSdata(Stream *s);

//Adafruit_SSD1306 display(-1);

const unsigned long DELAY = 5000;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

SoftwareSerial pmsSerial(2, 3);

Adafruit_7segment matrix = Adafruit_7segment();

struct pms5003data data;

void setup() {
  // our debugging output
  Serial.begin(9600);

  // sensor baud rate is 9600
  pmsSerial.begin(9600);

  // initialize and clear display
  /*
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  */
  matrix.begin(0x70);
  matrix.write(0x76); 
}


void printAqiOnMatrix(int aqi)
{
    // display a line of text
    matrix.clear();
    if (aqi > 10000) 
    {
      matrix.writeDigitRaw(0,0x23); // arrow UP
      matrix.writeDigitRaw(1,0x3E); // U
      matrix.writeDigitRaw(3,0x73); // P
      matrix.writeDigitRaw(4,0x23); // arrow UP
    }
    else
    {
      if (aqi < 1000) matrix.writeDigitRaw(0,0x77); // A
      else            matrix.writeDigitNum(0, (aqi/1000));
      
      if (aqi > 100)  matrix.writeDigitNum(1, (aqi%1000)/100);
      if (aqi > 10)   matrix.writeDigitNum(3, (aqi%100)/10);
      matrix.writeDigitNum(4, aqi%10); 
    }
    matrix.writeDisplay();
}

int convertPm25ToAqi(const int pm25)
{
  int result;
  // PM2.5 values
  int x[] = {0, 12,  35,  55, 150, 250, 350, 500};
  // AQI values
  int y[] = {0, 50, 100, 150, 200, 300, 400, 500};
  int size = sizeof(x)/sizeof(x[0]);
  
  if (pm25 >= x[size-1])
  {
    result = pm25;
  }
  else
  {
    // left nearest index
    int left = 0;
    for (int i = 0; i < size - 1; ++i)
    {
      if (pm25 >= x[i] && pm25 < x[i+1])
      {
        left = i;
        break; 
      }
    }
    
    result = y[left] + ((y[left + 1] - y[left])*(pm25 - x[left]))/(x[left + 1] - x[left]);
  }
  
  /*
  Serial.print("PM2.5 = ");Serial.print(pm25);
  Serial.print("\tindex = ");Serial.print(left);
  Serial.print("\tAQI = ");Serial.println(result);
  */
  return result;
}
int convertPm10ToAqi(const int pm25)
{
  int result;
  // PM2.5 values
  int x[] = {0, 55, 155, 255, 355, 425, 505, 605};
  // AQI values
  int y[] = {0, 50, 100, 150, 200, 300, 400, 500};
  int size = sizeof(x)/sizeof(x[0]);
  
  if (pm25 >= x[size-1])
  {
    result = pm25;
  }
  else
  {
    // left nearest index
    int left = 0;
    for (int i = 0; i < size - 1; ++i)
    {
      if (pm25 >= x[i] && pm25 < x[i+1])
      {
        left = i;
        break; 
      }
    }
    
    result = y[left] + ((y[left + 1] - y[left])*(pm25 - x[left]))/(x[left + 1] - x[left]);
  }
  
  /*
  Serial.print("PM2.5 = ");Serial.print(pm25);
  Serial.print("\tindex = ");Serial.print(left);
  Serial.print("\tAQI = ");Serial.println(result);
  */
  return result;
}

    
void loop() {
  static unsigned long lastUpdateTime = 0;
  if (readPMSdata(&pmsSerial)) 
  {
    // reading data was successful!
    //if (millis() - lastUpdateTime > DELAY || 0 == lastUpdateTime)
    {
//    Serial.println();
//    Serial.println("---------------------------------------");
      Serial.println("Concentration Units (standard, μg/m³)");
      Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
      Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
      Serial.print("\t\tPM 10: "); Serial.print(data.pm100_standard);
      Serial.print("\t\tAQI: "); Serial.println(convertPm25ToAqi(data.pm25_standard));
      
      int aqiPm25 = convertPm25ToAqi(data.pm25_standard);
      int aqiPm100 = convertPm10ToAqi(data.pm100_standard);
      printAqiOnMatrix(MAX(aqiPm25, aqiPm100));
      // display a line of text
      /*
      matrix.clear();
      matrix.writeDigitRaw(0,0x77); // A
      // matrix.print(convertPm25ToAqi(data.pm25_standard), DEC);
      
      int aqi = convertPm25ToAqi(data.pm25_standard);
      if (aqi/100 > 0) matrix.writeDigitNum(1, aqi/100);
      matrix.writeDigitNum(3, (aqi%100)/10);
      matrix.writeDigitNum(4, aqi%10); 
      
      matrix.writeDisplay();
      */
      /*
      display.clearDisplay();
      
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print(" AQI : "); display.println(convertPm25ToAqi(data.pm25_standard));
      display.print("PM1.0: "); display.println(data.pm10_standard);
      display.print("PM2.5: "); display.println(data.pm25_standard);
      display.print("PM 10: "); display.println(data.pm100_standard);

      display.display();
      */
      /*
      Serial.println("---------------------------------------");
      Serial.println("Concentration Units (environmental, μg/m³)");
      Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
      Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
      Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
      */
      Serial.println("---------------------------------------");
      Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
      Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
      Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
      Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
      Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
      Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
      Serial.println("---------------------------------------");
      

      lastUpdateTime = millis();
    }
  }
}

bool readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }

  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}

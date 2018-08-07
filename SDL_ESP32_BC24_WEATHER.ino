// Filename SDL_ESP32_BC24_WEATHER.ino
// Version 002 July 2018
// SwitchDoc Labs, LLC
//

//
//


#define BC24ESP32VERSION "002"

#define WEATHERBC24PUBNUBPROTOCOL "BC24WEATHER032"

// define DEBUGPRINT to print out lots of debugging information for WeatherPlus.

#define DEBUGPRINT

#undef PUBNUB_DEBUG



#define BC24DEBUG
#define BC24

#define BUTTONPIN 17

#define BLINKPIN 13

#define DEFAULTCLOCKTIMEOFFSETTOUTC -25200




// Change this to undef if you don't have the OLED present
#define OLED_Present

// BOF preprocessor bug prevent - insert on top of your arduino-code
#if 1
__asm volatile ("nop");
#endif

// Board options

#pragma GCC diagnostic ignored "-Wwrite-strings"
/*
  extern "C" {
  #include "user_interface.h"
  }
*/

#if defined(ARDUINO) && ARDUINO >= 100
// No extras
#elif defined(ARDUINO) // pre-1.0
// No extras
#elif defined(ESP_PLATFORM)
#include "arduinoish.hpp"
#endif


//#include "Time/TimeLib.h"
#include "TimeLib.h"

bool WiFiPresent = false;

#include <WiFi.h>


#include <esp_wps.h>
#include <esp_smartconfig.h>


#define ESP_WPS_MODE WPS_TYPE_PBC

// Kludge for latest ESP32 SDK - July 1, 2018

#define WPS_CONFIG_INIT_DEFAULT(type) { \
    .wps_type = type, \
                .crypto_funcs = &g_wifi_default_wps_crypto_funcs, \
  }

esp_wps_config_t config = WPS_CONFIG_INIT_DEFAULT(ESP_WPS_MODE);


#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include <TimeLib.h>

#include "Clock.h"

#include <Preferences.h>;

bool RTC_SetAndPresent = false;

#include "NTPClient.h"

WiFiUDP ntpUDP;


NTPClient timeClient(ntpUDP);

/* create an instance of Preferences library */
Preferences preferences;


#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

//gets called when WiFiManager enters configuration mode


void configModeCallback (WiFiManager *myWiFiManager)
{

  Serial.println("Entered config mode");

  Serial.println(WiFi.softAPIP());

}

// OTA updated
//#include <ESP8266WiFiMulti.h>
boolean AOK;  // 1=successful read
int heapSize;


#include "utility.h"








#include "config.h"

int pubNubEnabled;

String SDL2PubNubCode  = "";
String SDL2PubNubCode_Sub = "";


#define PUBLISHINTERVALSECONDS 30


#define PubNub_BASE_CLIENT WiFiClient

#define PUBNUB_DEFINE_STRSPN_AND_STRNCASECMP

#include "PubNub.h"

time_t lastBoot;


// parsing function
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



//

char channel1[]  = "OWIOT1";
char uuid[]   = WEATHERBC24PUBNUBPROTOCOL;








#include <Wire.h>

#include <Arduino.h> //needed for Serial.println

// debug the REST library

#define DEBUG_MODE 1

#include "aREST.h"

#include <String.h>





// display modes
#define DISPLAY_POWERUP 0
#define DISPLAY_IPDISPLAY 1
#define DISPLAY_WEATHER_SMALL 2
#define DISPLAY_WEATHER_MEDIUM 3
#define DISPLAY_WEATHER_LARGE 4
#define DISPLAY_STATUS 5
#define DISPLAY_ACCESSPOINT 6
#define DISPLAY_WEATHER_DEMO 7
#define DISPLAY_TRYING_AP 8
#define DISPLAY_FAILING_AP 9
#define DISPLAY_DATETIME 10
#define DISPLAY_UPDATING 11
#define DISPLAY_NO_UPDATE_AVAILABLE 12
#define DISPLAY_NO_UPDATE_FAILED 13
#define DISPLAY_UPDATE_FINISHED 14
#define DISPLAY_SUNAIRPLUS 16
#define DISPLAY_WXLINK 17
#define DISPLAY_SDL2PUBNUBCODE 18
#define DISPLAY_FAILED_RECONNECT 19
#define DISPLAY_LIGHTNING_STATUS 20
#define DISPLAY_LIGHTNING_DISPLAY 21
#define DISPLAY_TRYING_SMARTCONFIG 22
#define DISPLAY_TRYING_WPS 24

#define DEBUG


// Rest Interface


#define PREFIX ""

String RestTimeStamp;
String RestDataString;
String Version;






//----------------------------------------------------------------------
//Local WiFi


int WiFiSetupFlag = 0;

String APssid;





WiFiServer server(WEB_SERVER_PORT);

IPAddress myConnectedIp;
IPAddress myConnectedGateWay;
IPAddress myConnectedMask;


//----------------------------------------------------------------------


int blinkPin = 0;                // pin to blink led at each reading
// Create an instance of the server


// Create aREST instance
aREST rest = aREST();
// commands are functions that get called by the webserver framework
// they can read any posted data from client, and they output to server




#include "elapsedMillis.h"

elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop

elapsedMillis timeElapsed300Seconds; //declare global if you don't want it reset every time loop

// BMP180 / BMP280 Sensor
// Both are stored in BMP180 variables
//

#include "MAdafruit_BMP280.h"
#include "MAdafruit_BMP085.h"
Adafruit_BMP280 bme;

Adafruit_BMP085 bmp;
#define SENSORS_PRESSURE_SEALEVELHPA 1015.00
float altitude_meters;
float BMP180_Temperature;
float BMP180_Pressure;
float BMP180_Altitude;

bool BMP180Found;
bool BMP280Found;


int EnglishOrMetric;   // 0 = English units, 1 = Metric

int WeatherDisplayMode;



// HDC1080

#include "ClosedCube_HDC1080.h"

ClosedCube_HDC1080 hdc1080;

float HDC1080_Inside_Temperature;
float HDC1080_Inside_Humidity;

// AM2315

float AM2315_Temperature;
float AM2315_Humidity;
float dewpoint;

#include "SDL_ESP8266_HR_AM2315.h"


SDL_ESP8266_HR_AM2315 am2315;
float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]



const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

#include "AS3935.h"

// ThunderBoard AS3935 from SwitchDoc Labs
AS3935 as3935(0x02, 3);


// lightning state variables as3935

String as3935_LastLightning = "";
int as3935_LastLightningDistance = 0;
String as3935_LastEvent = "";
int as3935_LastReturnIRQ = 0;
String as3935_LastLightningTimeStamp = "";
String as3935_LastEventTimeStamp = "";
int as3835_LightningCountSinceBootup = 0;

String as3935_FullString = "";
String as3935_Params = "";

int as3935_NoiseFloor = 2;

bool as3935_Indoor = true;

int as3935_TuneCap = 7;
bool as3935_DisturberDetection = false;
int as3935_WatchdogThreshold = 3;
int as3935_SpikeDetection = 3;

bool AS3935Present = false;

void printAS3935Registers()
{
  int noiseFloor = as3935.getNoiseFloor();
  int spikeRejection = as3935.getSpikeRejection();
  int watchdogThreshold = as3935.getWatchdogThreshold();
  Serial.print("Noise floor is: ");
  Serial.println(noiseFloor, DEC);
  Serial.print("Spike rejection is: ");
  Serial.println(spikeRejection, DEC);
  Serial.print("Watchdog threshold is: ");
  Serial.println(watchdogThreshold, DEC);
}

int parseOutAS3935Parameters()
{

  // check for bad string

  if (as3935_Params.indexOf(",") == -1)
    as3935_Params = "2,1,7,0,3,3";

  String Value;

  Value = getValue(as3935_Params, ',', 0);
  if ((Value.toInt() < 0) || (Value.toInt() > 7))
    return 2;


  Value = getValue(as3935_Params, ',', 1);
  if ((Value.toInt() < 0) || (Value.toInt() > 1))
    return 2;

  Value = getValue(as3935_Params, ',', 2);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;

  Value = getValue(as3935_Params, ',', 3);
  if ((Value.toInt() < 0) || (Value.toInt() > 1))
    return 2;

  Value = getValue(as3935_Params, ',', 4);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;

  Value = getValue(as3935_Params, ',', 5);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;


  // OK, if we are here then all data is good
  Value = getValue(as3935_Params, ',', 0);
  as3935_NoiseFloor = Value.toInt();
  Value = getValue(as3935_Params, ',', 1);
  as3935_Indoor = Value.toInt();
  Value = getValue(as3935_Params, ',', 2);
  as3935_TuneCap = Value.toInt();
  Value = getValue(as3935_Params, ',', 3);
  as3935_DisturberDetection = Value.toInt();
  Value = getValue(as3935_Params, ',', 4);
  as3935_WatchdogThreshold = Value.toInt();
  Value = getValue(as3935_Params, ',', 5);
  as3935_SpikeDetection = Value.toInt();

  return 1;
}

void setAS3935Parameters()
{


  as3935.setTuningCapacitor(as3935_TuneCap);   // set to 1/2 - middle - you can calibrate on an Arduino UNO and use the value from there (pf/8)








  // lightning state variables as3935



  // first let's turn on disturber indication and print some register values from AS3935
  // tell AS3935 we are indoors, for outdoors use setOutdoors() function
  if (as3935_Indoor == true)
  {
    as3935.setIndoor();
  }
  else
  {
    as3935.setOutdoor();
  }

  as3935.setNoiseFloor(as3935_NoiseFloor);

#ifdef DEBUGPRINT
  Serial.print("NoiseFloor=");
  Serial.println(as3935_NoiseFloor);
#endif

  //AS3935.calibrate(); // can't calibrate because IRQ is polled and not through an Interrupt line on ESP8266

  // turn on indication of distrubers, once you have AS3935 all tuned, you can turn those off with disableDisturbers()

  if (as3935_DisturberDetection == true)
  {
    as3935.enableDisturbers();
  }
  else
  {
    as3935.disableDisturbers();
  }

  uint16_t getWatchdogThreshold(void);
  uint16_t setWatchdogThreshold(uint16_t wdth);


  as3935.setSpikeRejection(as3935_SpikeDetection);
  as3935.setWatchdogThreshold(as3935_WatchdogThreshold);

  // end set parameters


  // set up as3935 REST variable
  as3935_Params = String(as3935_NoiseFloor) + ",";
  as3935_Params += String(as3935_Indoor) + ",";
  as3935_Params += String(as3935_TuneCap) + ",";
  as3935_Params += String(as3935_DisturberDetection) + ",";
  as3935_Params += String(as3935_WatchdogThreshold) + ",";
  as3935_Params += String(as3935_SpikeDetection) ;


  printAS3935Registers();
}


// Station Name

String stationName;
String adminPassword;

// Health Indications for WeatherPlus



// WeatherUnderground

String WeatherUnderground_StationID;
String WeatherUnderground_StationKey;
int lastMessageID;



// WeatherRack


float windSpeedMin;
float windSpeedMax;
float windGustMin;
float windGustMax;
float windDirectionMin;
float windDirectionMax;



float currentWindSpeed;
float currentWindGust;
float currentWindDirection;

float rainTotal;


float rainCalendarDay;
int lastDay;

float startOfDayRain;

#include "SDL_RasPiGraphLibrary.h"
// setup the RasPiConnect Graph Arrays
SDL_RasPiGraphLibrary windSpeedGraph(10, SDL_MODE_LABELS);
SDL_RasPiGraphLibrary windGustGraph(10, SDL_MODE_LABELS);
SDL_RasPiGraphLibrary windDirectionGraph(10, SDL_MODE_LABELS);


char windSpeedBuffer[150];  // wind speed graph
char windGustBuffer[150];  // wind speed graph
char windDirectionBuffer[150];  // wind speed graph





// WeatherRack

// LED connected to digital GPIO 0
int WpinLED = 0;
// Anenometer connected to GPIO 14
int pinAnem = 14;
// Rain Bucket connected to  GPIO 15
int pinRain = 15;

#include "RTOSDefs.h"

#include "BC24Preferences.h";

#include "BigCircleFunctions.h"

#include "NeoFire.h"

#include "BC24WeatherLED.h"




#include "OWMAdafruit_ADS1015.h"


// include GET IP routines
#include "SDL_ESP32_BC24_GETIP.h"
Adafruit_ADS1015 ads1015(0x49);

int current_quality = -1;
Adafruit_ADS1115 adsAirQuality(0x48);



long currentAirQuality;
long currentAirQualitySensor;
int INTcurrentAirQualitySensor;
bool AirQualityPresent = false;

#include "AirQualitySensor.h"

#include "SDL_Weather_80422.h"


SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A7, SDL_MODE_INTERNAL_AD );
//SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A0, SDL_MODE_I2C_ESP32_0 );


// SDL_MODE_I2C_ADS1015
//

// RasPiConnect








long messageCount;

static uint8_t mac[] = LOCALMAC;
static uint8_t ip[] = LOCALIP;

// this is our current command object structure.  It is only valid inside void jsonCmd
typedef struct {
  char ObjectID[40];
  char ObjectFlags[40];
  char ObjectAction[40];
  char ObjectName[40];
  char ObjectServerID[40];
  char Password[40];
  char ObjectType[40];
  char Validate[40];
} currentObjectStructure;

char *md5str;

char ST1Text[40];   // used in ST-1 Send text control


char bubbleStatus[40];   // What to send to the Bubble status


#include "RainFunctions.h"

float lastRain;
#include "WeatherUnderground.h"







// OLED Constants


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// aREST functions

#include "aRestFunctions.h"


#include "SDL2PubNub.h"


// HDC1080
bool HDC1080_Present;


// SunAirPlus

bool SunAirPlus_Present;

float BatteryVoltage;
float BatteryCurrent;
float LoadVoltage;
float LoadCurrent;
float SolarPanelVoltage;
float SolarPanelCurrent;

// WXLink Support


#include "Crc16.h"

//Crc 16 library (XModem)
Crc16 crc;

bool WXLink_Present;


float WXBatteryVoltage;
float WXBatteryCurrent;

float WXLoadCurrent;
float WXSolarPanelVoltage;
float WXSolarPanelCurrent;
long WXMessageID;
bool WXLastMessageGood;


#include "WXLink.h"





#include "SDL_Arduino_INA3221.h"

SDL_Arduino_INA3221 SunAirPlus;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define LIPO_BATTERY_CHANNEL 1
#define SOLAR_CELL_CHANNEL 2
#define OUTPUT_CHANNEL 3

// OLED Display

#include "OWMAdafruit_GFX.h"
#include "ESP_SSD1306.h"


#define min(a,b) ((a)<(b)?(a):(b))

#define OLED_RESET 4



ESP_SSD1306 display(OLED_RESET);

#include "OLEDDisplay.h"

#include "OLEDLoopDisplay.h"


//
//

//
//


// validate temperature from AM2315 - Fixes the rare +16 degrees C issue
bool invalidTemperatureFound;

float validateTemperature(float incomingTemperature)
{
  if (incomingTemperature > AM2315_Temperature + 15.0) // check for large jump in temperature
  {
    // OK, we may have an invalid temperature.  Make sure this is not a startup (current humidity will be 0.0 if startup)
    if (AM2315_Humidity < 0.1)
    {
      // we are in startup phase, so accept temperature
      invalidTemperatureFound = false;
      return incomingTemperature;
    }
    else
    {
      // we have an issue with a bad read (typically a +32 degrees C increase)
      // so send last good temperature back and flag a bad temperature
      invalidTemperatureFound = true;
      return AM2315_Temperature;
    }
  }
  if (incomingTemperature < AM2315_Temperature - 15.0) // check for large decrease in temperature
  {
    // OK, we may have an invalid temperature.  Make sure this is not a startup (current humidity will be 0.0 if startup)
    if (AM2315_Humidity < 0.1)
    {
      // we are in startup phase, so accept temperature
      invalidTemperatureFound = false;
      return incomingTemperature;
    }
    else
    {
      // we have an issue with a bad read (typically a +32 degrees C increase)
      // so send last good temperature back and flag a bad temperature
      invalidTemperatureFound = true;
      return AM2315_Temperature;
    }
  }
  invalidTemperatureFound = false;
  return incomingTemperature; // good temperature

}


//scan for I2C Addresses

bool scanAddressForI2CBus(byte from_addr)
{
  byte error;

  // The i2c_scanner uses the return value of
  // the Write.endTransmisstion to see if
  // a device did acknowledge to the address.
  Wire.beginTransmission(from_addr);
  error = Wire.endTransmission();

  if (error == 0)
  {
    return true;
  }
  else if (error == 4)
  {

  }
  return false;
}




String returnDirectionFromDegrees(int degrees)
{

  if (degrees == 0)
    return "N";
  if (degrees == 22)
    return "NNE";
  if (degrees == 45)
    return "NE";
  if (degrees == 67)
    return "ENE";
  if (degrees == 90)
    return "E";
  if (degrees == 112)
    return "ESE";
  if (degrees == 135)
    return "SE";
  if (degrees == 157)
    return "SSE";
  if (degrees == 180)
    return "S";
  if (degrees == 202)
    return "SSW";
  if (degrees == 225)
    return "SW";
  if (degrees == 247)
    return "WSW";
  if (degrees == 270)
    return "W";
  if (degrees == 292)
    return "WNW";
  if (degrees == 315)
    return "NW";
  if (degrees == 337)
    return "NNW";


  return "XX";  // return previous value if not found

}







void blinkIPAddress()
{

  Serial.print("Blinking local Address =");

  Serial.println(WiFi.localIP());
  IPAddress LIP;
  LIP = WiFi.localIP();

  int lowOct;
  lowOct = LIP[3];

  // now do the blink

  int check100;
  int check10;
  int check1;
  delay(1000);

  check100 = lowOct / 100;

  Serial.println(check100);
  if (check100 != 0)
  {
    blinkLED(check100, 250);
  }

  delay(1000);

  check10 = (lowOct - check100 * 100) / 10;
  Serial.println(check10);
  if (check10 != 0)
  {
    blinkLED(check10, 500);
  }
  else
  {
    blinkLED(10, 100); // blink fast is 0

  }
  delay(1000);

  check1 = ( lowOct - check100 * 100 - check10 * 10);
  Serial.println(check1);
  if (check1 != 0)
  {
    blinkLED(check1, 500);
  }
  else
  {
    blinkLED(10, 100); // blink fast is 0

  }

}



void return2Digits(char returnString[], char *buffer2, int digits)
{
  if (digits < 10)
    sprintf(returnString, "0%i", digits);
  else
    sprintf(returnString, "%i", digits);

  strcpy(returnString, buffer2);


}

void buildTimeString(char returnString[], char *buffer2, tmElements_t convertTime)
{



  char myBuffer[5];
  sprintf(myBuffer, "%i-", tmYearToCalendar(convertTime.Year));
  strcat(returnString, myBuffer);

  return2Digits(myBuffer, myBuffer, convertTime.Month);
  strcat(returnString, myBuffer);
  strcat(returnString, "-");

  return2Digits(myBuffer, myBuffer, convertTime.Day);
  strcat(returnString, myBuffer);
  strcat(returnString, " ");

  return2Digits(myBuffer, myBuffer, convertTime.Hour);
  strcat(returnString, myBuffer);
  strcat(returnString, ":");

  return2Digits(myBuffer, myBuffer, convertTime.Minute);
  strcat(returnString, myBuffer);
  strcat(returnString, ":");

  return2Digits(myBuffer, myBuffer, convertTime.Second);
  strcat(returnString, myBuffer);



}

#include "esp_spi_flash.h"
#include "RTOSTasks.h"

void setup() {

#ifdef BC24DEBUG
  esp_log_level_set("*", ESP_LOG_VERBOSE);
#endif

  invalidTemperatureFound = false;


  // initalize our friend the BC24!
  //BC24inititialzeCircle();

  dumpSysInfo();
  getMaxMalloc(1 * 1024, 16 * 1024 * 1024);

  if (digitalLeds_initStrands(STRANDS, STRANDCNT)) {
    Serial.println("Init FAILURE: halting");
    while (true) {};
  }
  for (int i = 0; i < STRANDCNT; i++) {
    strand_t * pStrand = &STRANDS[i];
    Serial.print("Strand ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print((uint32_t)(pStrand->pixels), HEX);
    Serial.println();
#if DEBUG_ESP32_DIGITAL_LED_LIB
    dumpDebugBuffer(-2, digitalLeds_debugBuffer);
#endif
    digitalLeds_resetPixels(pStrand);
#if DEBUG_ESP32_DIGITAL_LED_LIB
    dumpDebugBuffer(-1, digitalLeds_debugBuffer);
#endif
  }
  Serial.println("BC24 Init complete");




  BMP180Found = false;
  BMP280Found = false;
  stationName = "";



  WeatherUnderground_StationID = "XXXX";
  WeatherUnderground_StationKey = "YYYY";

  adminPassword = "admin";
  altitude_meters = 637.0;  // default to 611

  pinMode(blinkPin, OUTPUT);        // pin that will blink every reading
  digitalWrite(blinkPin, HIGH);  // High of this pin is LED OFF


  Serial.begin(115200);           // set up Serial library at 9600 bps

  // set up button Pin
  pinMode (BUTTONPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

  Serial.print("Button=");
  Serial.println(digitalRead(BUTTONPIN));

  // Now check for clearing Preferences on hold down of Mode pin on reboot
  if (digitalRead(BUTTONPIN) == 0)
  {

    resetPreferences();
  }


  // setup preferences
  readPreferences();





  lastBoot = now();

  struct tm *lastBootTm;

  lastBootTm = localtime(&lastBoot);

  rainCalendarDay = 0.0;
  startOfDayRain = 0.0;

  lastDay = lastBootTm->tm_mday;

  String currentTimeString;





#ifdef OLED_Present
  OLEDDisplaySetup();
  updateDisplay(DISPLAY_POWERUP);
#endif

  delay(2000);

  if (digitalRead(0) == 0)
  {
    Serial.println("GPIO0 button down - reseting Preferences");
    resetPreferences();

  }




  readPreferences();

  setUpDarkLight();


  // now set up thunderboard AS3935



  // reset all internal register values to defaults
  as3935.reset();


  int noiseFloor = as3935.getNoiseFloor();

  Serial.print("noiseFloor=");
  Serial.println(noiseFloor);

  if (noiseFloor == 2)
  {
    Serial.println("AS3935 Present");
    AS3935Present = true;
  }
  else
  {
    Serial.println("AS3935 Not Present");
    AS3935Present = false;
  }

  if (AS3935Present == true)
  {

    parseOutAS3935Parameters();
    setAS3935Parameters();



  }

  // RTOS for WiFi support

  xSemaphoreSingleBlink = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreSingleBlink);   // initialize
  xSemaphoreTake( xSemaphoreSingleBlink, 10);   // start with this off



  xTaskCreatePinnedToCore(
    taskSingleBlink,          /* Task function. */
    "TaskSingleBlink",        /* String with name of task. */
    1000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  // Set up Wifi


  //---------------------
  // Setup WiFi Interface
  //---------------------
  // 5 cases:
  // 1) Use stored SSID/psk
  // 2) SmartConfig received
  // 3) WPS Button
  // 4) Local AP
  // 5) Wifi Fails


  WiFiPresent = false;
  WiFi.persistent(false);
  WiFi.begin();


  // check for SSID

  if (WiFi_SSID.length() != 0)
  {
    // use existing SSID
    Serial.println("Using existing SSID/psk");

    Serial.printf("SSID="); Serial.println(WiFi_SSID);
    Serial.printf("psk="); Serial.println(WiFi_psk);
    WiFi.begin(WiFi_SSID.c_str(), WiFi_psk.c_str());
    //Wait for WiFi to connect to AP
    Serial.println("Waiting for Saved WiFi");
#define WAITFORCONNECT 10
    for (int i = 0; i < WAITFORCONNECT * 2; i++)
    {
      if (WiFi.status() == WL_CONNECTED)
      {

        Serial.println("");
        Serial.println("WiFI Connected.");

        Serial.printf("SSID="); Serial.println(WiFi_SSID);
        Serial.printf("psk="); Serial.println(WiFi_psk);
        WiFiPresent = true;
#ifdef BC24
        BC24ThreeBlink(Green, 1000);
#endif

        break;
      }

      Serial.print(".");
      WiFiPresent = false;
      BC24ThreeBlink(White, 1000);
    }
    Serial.println();

  }


  if (WiFiPresent == false)
  {
    // do SmartConfig
#define WAITFORSTART 15
#define WAITFORCONNECT 20
    updateDisplay(DISPLAY_TRYING_SMARTCONFIG);

    WiFiPresent = SmartConfigGetIP(WAITFORSTART, WAITFORCONNECT);

  } // if not already programmed before





  if (WiFiPresent != true)
  {
#define WPSTIMEOUTSECONDS 30
    // now try WPS Button
    updateDisplay(DISPLAY_TRYING_WPS);
    WiFiPresent = WPSGetIP(WPSTIMEOUTSECONDS);

  }

  if (WiFiPresent != true)
  {
#define APTIMEOUTSECONDS 60
    updateDisplay(DISPLAY_TRYING_AP);
    WiFiPresent = localAPGetIP(APTIMEOUTSECONDS);
  }


  if (WiFiPresent == true)
  {

    WiFi_SSID = WiFi.SSID();
    WiFi_psk = WiFi.psk();
  }
  // write out preferences

  writePreferences();
  if (WiFiPresent == true)
  {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("WiFi Channel= ");
    Serial.println(WiFi.channel());
  }

  //---------------------
  // End Setup WiFi Interface
  //---------------------


  // Now do NTP (set's time to 00:00:00 if not present)  January 1, 1970


  // changed later with setTimeOffset() ). Additionaly you can specify the
  // update interval (in milliseconds, can be changed using setUpdateInterval() ).

  timeClient.setTimeOffset(ClockTimeOffsetToUTC);
  if (WiFiPresent == true)
  {
    timeClient.setUpdateInterval(3600000);
    timeClient.update();
  }
  time_t t;
  if (WiFiPresent == true)
  {
    t = timeClient.getEpochTime();
  }
  else
  {
    t = 1;
  }


  setTime(t);


  digitalClockDisplay();


  if (WiFi.status()  == WL_CONNECTED)

    WiFiPresent = true;


  writePreferences();

  Serial.print("WiFi Channel= ");
  Serial.println(WiFi.channel());

  blinkLED(2, 300);  // blink twice - OK!
  heapSize = ESP.getFreeHeap();

  RestTimeStamp = "";
  RestDataString = "";
  Version =   BC24ESP32VERSION;
  server.begin();

  rest.variable("BC24WeatherTime", &RestTimeStamp);
  rest.variable("FullDataString", &RestDataString);
  rest.variable("FirmwareVersion", &Version);

  rest.variable("currentLEDMode", &currentLEDMode);
  rest.variable("OLEDOn", &OLEDOn);
  rest.variable("darkLight", &darkLight);
  rest.variable("ClockTimeOffsetToUTC", &ClockTimeOffsetToUTC);

  rest.variable("BMPTemperature", &BMP180_Temperature);
  rest.variable("BarometricPressure", &BMP180_Pressure);
  rest.variable("Altitude", &BMP180_Altitude);


  rest.variable("InsideTemperature", &HDC1080_Inside_Temperature);
  rest.variable("InsideHumidity", &HDC1080_Inside_Humidity);

  rest.variable("OutdoorTemperature", &AM2315_Temperature);
  rest.variable("OutdoorHumidity", &AM2315_Humidity);

  rest.variable("CurrentWindSpeed", &currentWindSpeed);
  rest.variable("CurrentWindGust", &currentWindGust);

  rest.variable("CurrentWindDirection", &currentWindDirection);


  rest.variable("EnglishOrMetric", &EnglishOrMetric);

  rest.variable("RainTotal", &rainTotal);
  rest.variable("WindSpeedMin", &windSpeedMin);
  rest.variable("WindSpeedMax", &windSpeedMax);
  rest.variable("WindGustMin", &windGustMin);
  rest.variable("WindGustMax", &windGustMax);
  rest.variable("WindDirectionMin", &windDirectionMin);
  rest.variable("WindDirectionMax", &windDirectionMax);
  rest.variable("AirQualitySensor", &INTcurrentAirQualitySensor);



  // as3935 rest variables


  rest.variable("ThunderBoardLast", &as3935_FullString);
  rest.variable("ThunderBoardParams", &as3935_Params);




  // Handle REST calls
  WiFiClient client = server.available();
  if (client)
  {

    while (!client.available()) {
      delay(1);
    }
    if (client.available())
    {




      rest.handle(client);

    }
  }
  // health indications for device
  rest.variable("ESP32HeapSize", &heapSize);






  // Function to be exposed


  // auxillary functions
  rest.function("setID", setWeatherPlusIDControl);
  rest.function("resetBC24Weather",   resetBC24Weather);

  rest.function("setWUSID",   setWUSID);
  rest.function("setWUKEY",   setWUKEY);

  rest.function("setAdminPassword",   setAdminPassword);
  //rest.function("rebootBC24Weather",   rebootBC24Weather);
  rest.function("setDateTime",   setDateTime);
  rest.function("resetToDefaults",   resetToDefaults);

  rest.function("resetWiFiAccessPoint", resetWiFiAccessPoint);
  //rest.function("updateBC24Weather", updateBC24Weather);



  // external interfaces

  rest.function("enableCWOP", enableCWOPControl);
  rest.function("enableTwitter", enableTwitterControl);

  // weather functions

  rest.function("WeatherSmall",   weatherSmallControl);
  rest.function("WeatherMedium",   weatherMediumControl);
  rest.function("WeatherLarge",   weatherLargeControl);
  rest.function("WeatherDemo",   weatherDemoControl);

  rest.function("EnglishUnits",   englishUnitControl);

  rest.function("MetricUnits",   metricUnitControl);

  // PubNub

  rest.function("EnablePubNub", enableDisableSDL2PubNub);

  rest.function("SendPubNubState", sendStateSDL2PubNub);


  // Thunderboard functions AS3935



  rest.function("setThunderBoardParams", setThunderBoardParams);


  // BC24 Display Functions


  rest.function("setCurrentLEDMode", setCurrentLEDMode);
  rest.function("setOLEDOn", setOLEDOn);
  rest.function("setDarkLight", setDarkLight);
  rest.function("setClockTimeOffsetToUTC", setClockTimeOffsetToUTC);


  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("BC24Weather");



  initialize60MinuteRain();


  Serial.println();
  Serial.println();
  Serial.println("--------------------");
  Serial.println("BC24 Weather ESP32");
  Serial.println("--------------------");
  Serial.print("Version: ");
  Serial.println(BC24ESP32VERSION);

  Serial.print("Compiled at:");
  Serial.print (__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);


  // Free heap on ESP32
  Serial.print("Free Heap Space on BC24:");
  Serial.println(ESP.getFreeHeap());

#ifdef BC24DEBUG
  Serial.print("CPU0 reset reason: ");
  print_reset_reason(rtc_get_reset_reason(0));

  Serial.print("CPU1 reset reason: ");
  print_reset_reason(rtc_get_reset_reason(1));
#endif

  Serial.print("RAM left: ");
  Serial.println(system_get_free_heap_size());

  // RTOS


  xSemaphoreClock = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreClock);   // initialize
  xSemaphoreTake( xSemaphoreClock, 10);   // start with this off

  xSemaphoreFire = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreFire);   // initialize
  xSemaphoreTake( xSemaphoreFire, 10);   // start with this off

  xSemaphoreRESTCommand = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreRESTCommand);   // initialize
  //xSemaphoreTake( xSemaphoreRESTCommand, 10);   // start with this off

  xSemaphoreOLEDLoopUpdate = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreOLEDLoopUpdate);   // initialize
  xSemaphoreTake( xSemaphoreOLEDLoopUpdate, 10);   // start with this off

  xSemaphoreUpdate = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreUpdate);   // initialize with on

  xSemaphoreRainbow = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreRainbow);   // initialize
  xSemaphoreTake( xSemaphoreRainbow, 10);   // start with this off

  xSemaphoreWind = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreWind);   // initialize
  xSemaphoreTake( xSemaphoreWind, 10);   // start with this off

  Serial.println("RTOS Tasks Starting");

  xTaskCreatePinnedToCore(
    taskButtonRead,          /* Task function. */
    "TaskButtonRead",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core


  xTaskCreatePinnedToCore(
    taskClock,          /* Task function. */
    "TaskClock",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  xTaskCreatePinnedToCore(
    taskUpdate,          /* Task function. */
    "TaskUpdate",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  xTaskCreatePinnedToCore(
    taskFire,          /* Task function. */
    "TaskFire",        /* String with name of task. */
    2000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  xTaskCreatePinnedToCore(
    taskRESTCommand,          /* Task function. */
    "TaskRESTCommand",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    3,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

  xTaskCreatePinnedToCore(
    taskWind,          /* Task function. */
    "TaskWind",        /* String with name of task. */
    2000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);

  xTaskCreatePinnedToCore(
    taskRainbow,          /* Task function. */
    "TaskRainbow",        /* String with name of task. */
    2000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

#ifdef OLEDEXPERIMENT
  xTaskCreatePinnedToCore(
    taskOLEDLoopUpdate,          /* Task function. */
    "TaskOLEDLoopUpdate",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core

#endif
  // test for SunAirPlus_Present
  SunAirPlus_Present = false;

  LoadVoltage = SunAirPlus.getBusVoltage_V(OUTPUT_CHANNEL);

  if (LoadVoltage < 0.1)
  {
    SunAirPlus_Present = false;
    Serial.println("SunAirPlus Not Present");
  }
  else
  {
    SunAirPlus_Present = true;
    Serial.println("SunAirPlus Present");
  }

  // test for HDC1080

  HDC1080_Present = false;

  HDC1080_Present = scanAddressForI2CBus(0x40);

  if (HDC1080_Present)
  {
    Serial.println("HDC1080 Present");
    // Default settings:
    //  - Heater off
    //  - 14 bit Temperature and Humidity Measurement Resolutions
    hdc1080.begin(0x40);

  }
  else
    Serial.println("HDC1080 NOT Present");

  // test for WXLink Present

  WXLink_Present = false;

  WXLink_Present = scanAddressForI2CBus(0x08);

  WXLastMessageGood = false;

  WXMessageID = 0;
  WXLoadCurrent = 0.0;


  WXBatteryVoltage = 0.0;
  WXBatteryCurrent = 0.0;

  WXSolarPanelVoltage = 0.0;
  WXSolarPanelCurrent = 0.0;
  lastMessageID = -1;

  if (WXLink_Present == false)
  {

    Serial.println("WXLink Not Present");
  }
  else
  {

    Serial.println("WXLink Present");
  }


  Serial.print("port number = ");
  Serial.println(WEB_SERVER_PORT);

  delay(2000);

  ads1015.begin();

  adsAirQuality.begin();


  int16_t ad0 = adsAirQuality.readADC_SingleEnded(0);



  currentAirQuality = -1;
  currentAirQualitySensor = 0;
  INTcurrentAirQualitySensor = 0;

  if (ad0 != -1)
  {
    AirQualityPresent = true;
    Serial.println("AirQuality Extension Present");
  }
  else
  {
    AirQualityPresent = false;
    Serial.println("AirQuality Extension Not Present");
  }


  randomSeed(analogRead(0));

  lastBoot = now();



  Serial.print("BC24Weather IP Address:");
  Serial.println(WiFi.localIP());
  myConnectedIp = WiFi.localIP();

  Serial.print("BC24Weather Gateway Address:");
  myConnectedGateWay = WiFi.gatewayIP();
  Serial.println(WiFi.gatewayIP());

  Serial.print("BC24Weather subnet Mask:");
  myConnectedMask = WiFi.subnetMask();
  Serial.println(WiFi.subnetMask());


  //blinkIPAddress();

  updateDisplay(DISPLAY_IPDISPLAY);

  // Now put PUBNUB Code up there

  updateDisplay(DISPLAY_SDL2PUBNUBCODE);

  delay(1000);
  display.clearDisplay();

  display.display();

  timeElapsed = 0;

  // BMP180

  /* Initialise the sensor */
  if (!bmp.begin())
  {
    /* There was a problem detecting the BM180 ... check your connections */
    Serial.println("No BMP180 detected ");
    BMP180Found = false;

  }
  else
  {
    Serial.println("BMP180 Detected");
    BMP180Found = true;
  }

  // BMP280

  /* Initialise the sensor */
  if (!bme.begin())
  {

    Serial.println("No BMP280 detected ");
    BMP280Found = false;

  }
  else
  {
    Serial.println("BMP280 Detected");
    BMP280Found = true;
  }



  // AM2315

  // setup AM2315


  AOK = am2315.readData(dataAM2315);


  if (AOK) {

    Serial.println("AM2315 Detected...");

    //Serial.print("Hum: "); Serial.println(dataAM2315[1]);
    //Serial.print("TempF: "); Serial.println(dataAM2315[0]);


    AM2315_Temperature = dataAM2315[1];
    AM2315_Humidity = dataAM2315[0];
    dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);

  }
  else
  {

    Serial.println("AM2315 Sensor not found, check wiring & pullups!");

  }


  AM2315_Temperature = 0.0;

  AM2315_Humidity = 0.0;
  dewpoint = 0.0;


  if (WiFiPresent == true)
  {

    PubNub.begin(SDL2PubNubCode.c_str(), SDL2PubNubCode_Sub.c_str());

    Serial.println("PubNub set up");
  }






}


//
//
// loop()
//
//

bool firstTime = true;




void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Starting Main Loop");
  /*
    // Handle REST calls
    WiFiClient client = server.available();

    int timeout;
    timeout = 0;
    if (client)
    {

    // Thank you to MAKA69 for this suggestion
    while (!client.available()) {
      Serial.print(".");
      delay(1);
      timeout++;
      if (timeout > 1000) {
        Serial.print("INFINITE LOOP BREAK!");
        break;
      }
    }
    if (client.available())
    {




      rest.handle(client);

    }
    }
    client.stop();

  */

  // if (timeElapsed > 5000)
  if (timeElapsed > 2000)
  {
#ifdef OLEDEXPERIMENT
    Serial.print("xSemaphoreOLEDLoopUpdate1="); Serial.println(uxSemaphoreGetCount( xSemaphoreOLEDLoopUpdate ));
    if (firstTime == false)
    {
      xSemaphoreTake( xSemaphoreOLEDLoopUpdate, portMAX_DELAY);   // stop OLED display updating
      firstTime = false;
    }
    Serial.print("xSemaphoreOLEDLoopUpdate2="); Serial.println(uxSemaphoreGetCount( xSemaphoreOLEDLoopUpdate ));
#endif

    //   Serial.println("5 second Loop executed");

    Serial.println("2 second Loop executed");

    // Update all Weather Variables

    // block display updates during data refresh

    xSemaphoreTake( xSemaphoreUpdate, 10);

    timeElapsed = 0;

    Serial.print("Free heap on ESP32:");
    heapSize = ESP.getFreeHeap();
    Serial.println(heapSize, DEC);


    tmElements_t tm;
    Serial.println("---------------");
    Serial.println("Internal Clock");
    Serial.println("---------------");

    //RtcDateTime now = Rtc.GetDateTime();

    String currentTimeString;
    currentTimeString = returnDateTime(now());


    Serial.println(currentTimeString);

    RestTimeStamp = currentTimeString;

    RestDataString = "";


    Serial.println("---------------");
    Serial.println("AM2315");
    Serial.println("---------------");


    if (!WXLink_Present)
    {

      AOK = am2315.readData(dataAM2315);
#ifdef DEBUGPRINT
      Serial.print("AOK=");
      Serial.println(AOK);
#endif
      AM2315_Temperature = dataAM2315[1];
      AM2315_Humidity = dataAM2315[0];
      dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);

      Serial.print("Temp: "); Serial.println(AM2315_Temperature);
      Serial.print("Hum: "); Serial.println(AM2315_Humidity);
      Serial.print("DwPt: "); Serial.println(dewpoint);
#ifdef DEBUGPRINT
      am2315.printStatistics();
#endif
    }
    else
    {
      Serial.println("WXLink Present - AM2315 local read overruled");
    }


    RestDataString += String(AM2315_Temperature, 2) + ",";
    RestDataString += String(AM2315_Humidity, 2) + ",";

    Serial.println("---------------");
    if (BMP180Found)
      Serial.println("BMP180");
    else if (BMP280Found)
      Serial.println("BMP280");
    else
      Serial.println("No BMP180/BMP280 Found");
    Serial.println("---------------");


    if (BMP180Found)
    {


      /* Display the results (barometric pressure is measure in hPa) */
      //BMP180_Pressure = bmp.readPressure();
      // Put Alitude in Meters
      BMP180_Pressure = bmp.readSealevelPressure(altitude_meters);
      /* Display atmospheric pressue in hPa */
      Serial.print("Pressure:    ");
      Serial.print(BMP180_Pressure / 100.0);
      Serial.println(" kPa");



      /* Calculating altitude with reasonable accuracy requires pressure    *
         sea level pressure for your position at the moment the data is
         converted, as well as the ambient temperature in degress
         celcius.  If you don't have these values, a 'generic' value of
         1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA
         in sensors.h), but this isn't ideal and will give variable
         results from one day to the next.
       *                                                                    *
         You can usually find the current SLP value by looking at weather
         websites or from environmental information centers near any major
         airport.
       *                                                                    *
         For example, for Paris, France you can check the current mean
         pressure and sea level at: http://bit.ly/16Au8ol                   */

      /* First we get the current temperature from the BMP085 */
      float temperature;
      temperature = bmp.readTemperature();
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");


      BMP180_Temperature = temperature;




      /* Then convert the atmospheric pressure, and SLP to altitude         */
      /* Update this next line with the current SLP for better results      */
      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      float altitude;
      altitude = bmp.readAltitude(BMP180_Pressure);
      Serial.print("Altitude:    ");
      Serial.print(altitude);

      BMP180_Altitude = altitude;
      Serial.println(" m");

    }



    if (BMP280Found)
    {

      /* Display the results (barometric pressure is measure in hPa) */
      //BMP180_Pressure = bmp.readPressure();
      // Put Alitude in Meters

      BMP180_Pressure = bme.readSealevelPressure(altitude_meters);

      /* Display atmospheric pressue in hPa */
      Serial.print("Pressure:    ");
      Serial.print(BMP180_Pressure / 100.0);
      Serial.println(" hPa");



      /* Calculating altitude with reasonable accuracy requires pressure    *
         sea level pressure for your position at the moment the data is
         converted, as well as the ambient temperature in degress
         celcius.  If you don't have these values, a 'generic' value of
         1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA
         in sensors.h), but this isn't ideal and will give variable
         results from one day to the next.
       *                                                                    *
         You can usually find the current SLP value by looking at weather
         websites or from environmental information centers near any major
         airport.
       *                                                                    *
         For example, for Paris, France you can check the current mean
         pressure and sea level at: http://bit.ly/16Au8ol                   */

      /* First we get the current temperature from the BMP085 */
      float temperature;
      temperature = bme.readTemperature();
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");


      BMP180_Temperature = temperature;




      /* Then convert the atmospheric pressure, and SLP to altitude         */
      /* Update this next line with the current SLP for better results      */
      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      float altitude;
      altitude = bme.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
      Serial.print("Altitude:    ");
      Serial.print(altitude);

      BMP180_Altitude = altitude;
      Serial.println(" m");

    }

    RestDataString += String(BMP180_Temperature, 2) + ",";
    RestDataString += String(BMP180_Pressure, 2) + ",";
    RestDataString += String(BMP180_Altitude, 2) + ",";



    if (AirQualityPresent)
    {

      Serial.println("---------------");
      Serial.println("AirQualitySensor");
      Serial.println("---------------");


#ifdef DEBUGPRINT

      int16_t ad0 = adsAirQuality.readADC_SingleEnded(0);
      int16_t ad1 = adsAirQuality.readADC_SingleEnded(1);
      int16_t ad2 = adsAirQuality.readADC_SingleEnded(2);
      int16_t ad3 = adsAirQuality.readADC_SingleEnded(3);

      Serial.print("ad0=");
      Serial.println(ad0);
      Serial.print("ad1=");
      Serial.println(ad1);
      Serial.print("ad2=");
      Serial.println(ad2);
      Serial.print("ad3=");

      Serial.println(ad3);



      int16_t ad0_0x49 = ads1015.readADC_SingleEnded(0);
      int16_t ad1_0x49 = ads1015.readADC_SingleEnded(1);
      int16_t ad2_0x49 = ads1015.readADC_SingleEnded(2);
      int16_t ad3_0x49 = ads1015.readADC_SingleEnded(3);

      Serial.print("ad0_0x49=");
      Serial.println(ad0_0x49);
      Serial.print("ad1_0x49=");
      Serial.println(ad1_0x49);
      Serial.print("ad2_0x49=");
      Serial.println(ad2_0x49);
      Serial.print("ad3_0x49=");
      Serial.println(ad3_0x49);
#endif

      currentAirQuality = getAirQuality();

    }

    Serial.println("---------------");
    if (SunAirPlus_Present)
      Serial.println("SunAirPlus");
    else
      Serial.println("SunAirPlus Not Present");
    Serial.println("---------------");

    // if SunAirPlus present, read charge data

    if (SunAirPlus_Present)
    {

      LoadVoltage = SunAirPlus.getBusVoltage_V(OUTPUT_CHANNEL);
      LoadCurrent = SunAirPlus.getCurrent_mA(OUTPUT_CHANNEL);


      BatteryVoltage = SunAirPlus.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
      BatteryCurrent = SunAirPlus.getCurrent_mA(LIPO_BATTERY_CHANNEL);

      SolarPanelVoltage = SunAirPlus.getBusVoltage_V(SOLAR_CELL_CHANNEL);
      SolarPanelCurrent = -SunAirPlus.getCurrent_mA(SOLAR_CELL_CHANNEL);

#ifdef DEBUGPRINT
      Serial.println("");
      Serial.print("LIPO_Battery Load Voltage:  "); Serial.print(BatteryVoltage); Serial.println(" V");
      Serial.print("LIPO_Battery Current:       "); Serial.print(BatteryCurrent); Serial.println(" mA");
      Serial.println("");

      Serial.print("Solar Panel Voltage:   "); Serial.print(SolarPanelVoltage); Serial.println(" V");
      Serial.print("Solar Panel Current:   "); Serial.print(SolarPanelCurrent); Serial.println(" mA");
      Serial.println("");

      Serial.print("Load Voltage:   "); Serial.print(LoadVoltage); Serial.println(" V");
      Serial.print("Load Current:   "); Serial.print(LoadCurrent); Serial.println(" mA");
      Serial.println("");
#endif

    }
    else
    {

      LoadVoltage = 0.0;
      LoadCurrent = 0.0;


      BatteryVoltage = 0.0;
      BatteryCurrent = 0.0;

      SolarPanelVoltage = 0.0;
      SolarPanelCurrent = 0.0;


    }




    Serial.println("---------------");
    if (WXLink_Present)
      Serial.println("WXLink");
    else
      Serial.println("WXLink Not Present");
    Serial.println("---------------");

    // read variables from WXLink

    if (WXLink_Present)
    {
      if (readWXLink() == true)
      {
        WXLastMessageGood = true;
        blinkLED(2, 200);  // blink 2 for good message


      }
      else
      {

        WXLastMessageGood = false;



      }
    }

    Serial.println("---------------");
    if (HDC1080_Present)
      Serial.println("HDC1080 Inside Temp/Humidity Present");
    else
      Serial.println("HDC1080 Inside Temp/Humidity Not Present");
    Serial.println("---------------");


    if (HDC1080_Present)
    {


      HDC1080_Inside_Temperature = hdc1080.readTemperature();
      HDC1080_Inside_Humidity = hdc1080.readHumidity();
      Serial.print("HDC1080 Inside Temperature:");
      Serial.print(HDC1080_Inside_Temperature);
      Serial.println(" Degrees C");
      Serial.print("HDC1080 Inside Humdity:");
      Serial.print(HDC1080_Inside_Humidity);
      Serial.println(" %");

    }
    else
    {
      HDC1080_Inside_Temperature = 0.0;
      HDC1080_Inside_Humidity = 0.0;

    }



    Serial.println("---------------");
    Serial.println("WeatherRack");
    Serial.println("---------------");

    if (WXLink_Present == false)
    {

      currentWindSpeed = weatherStation.current_wind_speed();
      currentWindGust = weatherStation.get_wind_gust();

      currentWindDirection = weatherStation.current_wind_direction();

      float oldRain = rainTotal;
      rainTotal = rainTotal + weatherStation.get_current_rain_total();
      if (oldRain < rainTotal)
      {
        strcpy(bubbleStatus, "It is Raining\0");
      }

      windSpeedGraph.add_value(currentWindSpeed);
      windGustGraph.add_value(currentWindGust);
      windDirectionGraph.add_value(currentWindDirection);

      windSpeedGraph.getRasPiString(windSpeedBuffer, windSpeedBuffer);
      windGustGraph.getRasPiString(windGustBuffer, windGustBuffer);
      windDirectionGraph.getRasPiString(windDirectionBuffer, windDirectionBuffer);

      windSpeedMin = windSpeedGraph.returnMinValue();
      windSpeedMax = windSpeedGraph.returnMaxValue();
      windGustMin = windGustGraph.returnMinValue();
      windGustMax = windGustGraph.returnMaxValue();
      windDirectionMin = windDirectionGraph.returnMinValue();
      windDirectionMax = windDirectionGraph.returnMaxValue();
    }
    else
    {
      // WXLink is PRESENT, take from WXLink

      if (WXLastMessageGood == true)  // if bad WX Message, don't change
      {

        currentWindSpeed = convert4BytesToFloat(buffer, 9);
        currentWindGust = convert4BytesToFloat(buffer, 21);

        currentWindDirection = convert2BytesToInt(buffer, 7);

        float oldRain = rainTotal;
        rainTotal = convert4BytesToLong(buffer, 17);

        if (oldRain < rainTotal)
        {
          strcpy(bubbleStatus, "It is Raining\0");
        }

        windSpeedGraph.add_value(currentWindSpeed);
        windGustGraph.add_value(currentWindGust);
        windDirectionGraph.add_value(currentWindDirection);

        windSpeedGraph.getRasPiString(windSpeedBuffer, windSpeedBuffer);
        windGustGraph.getRasPiString(windGustBuffer, windGustBuffer);
        windDirectionGraph.getRasPiString(windDirectionBuffer, windDirectionBuffer);

        windSpeedMin = windSpeedGraph.returnMinValue();
        windSpeedMax = windSpeedGraph.returnMaxValue();
        windGustMin = windGustGraph.returnMinValue();
        windGustMax = windGustGraph.returnMaxValue();
        windDirectionMin = windDirectionGraph.returnMinValue();
        windDirectionMax = windDirectionGraph.returnMaxValue();

        // Now overwrite outside temp/humidity

        AM2315_Temperature = validateTemperature(convert4BytesToFloat(buffer, 25));
        AM2315_Humidity = convert4BytesToFloat(buffer, 29);

        // calculate dewpoint
        dewpoint =  AM2315_Temperature - ((100.0 - AM2315_Humidity) / 5.0);


        // set up solar status and message ID for screen

        // if WXLINK present, read charge data



        WXLoadCurrent = convert4BytesToFloat(buffer, 41);


        WXBatteryVoltage = convert4BytesToFloat(buffer, 33);
        WXBatteryCurrent = convert4BytesToFloat(buffer, 37);

        WXSolarPanelVoltage = convert4BytesToFloat(buffer, 45);
        WXSolarPanelCurrent = convert4BytesToFloat(buffer, 49);

        WXMessageID = convert4BytesToLong(buffer, 57);

        /*   Serial.println("");
           Serial.print("WXLIPO_Battery Load Voltage:  "); Serial.print(WXBatteryVoltage); Serial.println(" V");
           Serial.print("WXLIPO_Battery Current:       "); Serial.print(WXBatteryCurrent); Serial.println(" mA");
           Serial.println("");

           Serial.print("WXSolar Panel Voltage:   "); Serial.print(WXSolarPanelVoltage); Serial.println(" V");
           Serial.print("WXSolar Panel Current:   "); Serial.print(WXSolarPanelCurrent); Serial.println(" mA");
           Serial.println("");

           Serial.print("WXLoad Current:   "); Serial.print(WXLoadCurrent); Serial.println(" mA");
           Serial.println("");
        */




      }




    }







    Serial.print("windSpeedMin =");
    Serial.print(windSpeedMin);
    Serial.print(" windSpeedMax =");
    Serial.println(windSpeedMax);
#ifdef DEBUGPRINT
    Serial.print("windSpeedBuffer=");
    Serial.println(windSpeedBuffer);
#endif
    Serial.print("windGustMin =");
    Serial.print(windGustMin);
    Serial.print(" windGustMax =");
    Serial.println(windGustMax);
#ifdef DEBUGPRINT
    Serial.print("windGustBuffer=");
    Serial.println(windGustBuffer);
#endif
    Serial.print("windDirectionMin =");
    Serial.print(windDirectionMin);
    Serial.print(" windDirectionMax =");
    Serial.println(windDirectionMax);

#ifdef DEBUGPRINT
    Serial.print("windDirectionBuffer=");
    Serial.println(windDirectionBuffer);
#endif
    Serial.print("currentWindSpeed=");
    Serial.print(currentWindSpeed);

    Serial.print(" \tcurrentWindGust=");
    Serial.print (currentWindGust);

    Serial.print(" \tWind Direction=");
    Serial.print(currentWindDirection);


    Serial.print(" \t\tCumulative Rain = ");
    Serial.println(rainTotal);


    Serial.println(" ");



    RestDataString += String(currentWindSpeed, 2) + ",";
    RestDataString += String(currentWindGust, 2) + ",";
    RestDataString += String(currentWindDirection, 2) + ",";
    RestDataString += String(rainTotal, 2) + ",";
    RestDataString += String(windSpeedMin, 2) + ",";
    RestDataString += String(windSpeedMax, 2) + ",";
    RestDataString += String(windGustMin, 2) + ",";
    RestDataString += String(windGustMax, 2) + ",";

    RestDataString += String(windDirectionMin, 2) + ",";
    RestDataString += String(windDirectionMax, 2) + ",";
    RestDataString += String(EnglishOrMetric) + "," ;
    RestDataString += currentTimeString + "," ;
    RestDataString += stationName + ",";
    RestDataString += String(currentAirQualitySensor) + ",";
    RestDataString += String(currentAirQuality) + ",";

    RestDataString += String(BatteryVoltage, 2) + ",";
    RestDataString += String(BatteryCurrent, 2) + ",";
    RestDataString += String(SolarPanelVoltage, 2) + ",";
    RestDataString += String(SolarPanelCurrent, 2) + ",";
    RestDataString += String(LoadVoltage, 2) + ",";
    RestDataString += String(LoadCurrent, 2) + ",";


    RestDataString += String(WXBatteryVoltage, 2) + ",";
    RestDataString += String(WXBatteryCurrent, 2) + ",";
    RestDataString += String(WXSolarPanelVoltage, 2) + ",";
    RestDataString += String(WXSolarPanelCurrent, 2) + ",";
    RestDataString += "0.00,";
    RestDataString += String(WXLoadCurrent, 2) + ",";

    if (invalidTemperatureFound == true)
    {
      RestDataString += "IVF:" + String(AOK) + ",";
    }
    else
    {
      RestDataString += "V:" + String(AOK) + ",";
    }
    invalidTemperatureFound = false;


    // Restart WiFi in case of connected, then lost connection
    if (WiFiPresent == true)
    {
      if (WiFi.status()  != WL_CONNECTED)

      {




      }
    }


    if (WXLastMessageGood == true)
    {
      RestDataString += "WXLMG ,";
    }
    else
    {
      RestDataString += "WXLMB ,";
    }

    RestDataString += String(pubNubEnabled) + ",";


    if (AS3935Present == true)
    {
      // Now check for Lightning ThunderBoard AS3935

      Serial.println("---------------");
      Serial.println("ThunderBoard AS3935 Lightning Detector");
      Serial.println("---------------");

      // first step is to find out what caused interrupt
      int strokeDistance = 0.0;
      int irqSource = 0;

      irqSource = as3935.getInterruptReason();
      Serial.print("as3935 irqSource: ");
      Serial.println(irqSource, HEX);

      Serial.print("as3935 irqSource: ");
      Serial.println(irqSource, BIN);




      if (irqSource > 0)
      {

        printAS3935Registers();
        as3935_LastReturnIRQ = irqSource;
        // returned value is bitmap field, bit 0 - noise level too high, bit 2 - disturber detected, and finally bit 3 - lightning!
        if (irqSource & 0b0001)
        {
          Serial.println("INT_NH Interrupt: Noise level too high, try adjusting noise floor");

          as3935_LastEvent = "Noise Level too high";

          as3935_LastEventTimeStamp = returnDateTime(now());
        }
        if (irqSource & 0b0100)
        {
          Serial.println("INT_D Interrupt: Disturber detected");
          as3935_LastEvent = "Disturber detected";

          as3935_LastEventTimeStamp = returnDateTime(now());
        }
        if (irqSource & 0b1000)
        {
          // need to find how far that lightning stroke, function returns approximate distance in kilometers,
          // where value 1 represents storm in detector's near victinity, and 63 - very distant, out of range stroke
          // everything in between is just distance in kilometers

          strokeDistance = as3935.getDistance();

          as3935_LastEvent = "Lightning detected";

          as3935_LastEventTimeStamp = returnDateTime(now());

          as3935_LastLightning  = String(strokeDistance) + " km" ;
          as3935_LastLightningTimeStamp = returnDateTime(now());
          as3935_LastLightningDistance = strokeDistance;
          as3835_LightningCountSinceBootup++;


          Serial.print("INT_L Interrupt: Lightning Detected.  Stroke Distance:");
          Serial.print(strokeDistance);
          Serial.println(" km");
          if (strokeDistance == 1)
            Serial.println("Storm overhead");
          if (strokeDistance == 63)
            Serial.println("Out of range lightning detected.");

          delay(3000);
          updateDisplay(DISPLAY_LIGHTNING_DISPLAY);
          delay(3000);




        }
      }

    }
    //  Lightning REST variable
    as3935_FullString = "";
    as3935_FullString += as3935_LastLightning + ",";
    as3935_FullString += as3935_LastLightningTimeStamp + ",";
    as3935_FullString += String(as3935_LastLightningDistance) + ",";
    as3935_FullString += as3935_LastEvent + ",";
    as3935_FullString += as3935_LastEventTimeStamp + ",";
    as3935_FullString += String(as3835_LightningCountSinceBootup);


    // Lighting Rest
    RestDataString += as3935_LastLightning + ",";
    RestDataString += as3935_LastLightningTimeStamp + ",";
    RestDataString += String(as3935_LastLightningDistance) + ",";
    RestDataString += as3935_LastEvent + ",";
    RestDataString += as3935_LastEventTimeStamp + ",";
    RestDataString += String(as3835_LightningCountSinceBootup) + ",";

    RestDataString += String(HDC1080_Inside_Temperature, 2) + ",";
    RestDataString += String(HDC1080_Inside_Humidity, 2);

    xSemaphoreGive( xSemaphoreUpdate);
#ifdef OLEDEXPERIMENT
    xSemaphoreGive( xSemaphoreOLEDLoopUpdate);   // start display

    Serial.print("xSemaphoreOLEDLoopUpdate="); Serial.println(uxSemaphoreGetCount( xSemaphoreOLEDLoopUpdate ));
#else
    MainWeatherOLEDLoopDisplay();
#endif

    if (timeElapsed300Seconds > 300000)   // 5 minutes
    {


      String lastBootTimeString;
      lastBootTimeString = returnDateTime(lastBoot);

      Serial.print("lastBoot = ");
      Serial.println(lastBootTimeString);

      timeElapsed300Seconds = 0;

      // update rain


      add60MinuteRainReading(rainTotal - lastRain);
      lastRain = rainTotal;

      struct tm *myTMNow;
      time_t myNow;
      myNow = now();
      myTMNow = localtime(&myNow);



      if (myTMNow->tm_mday == lastDay)
      {
        rainCalendarDay = rainTotal - startOfDayRain;

      }
      else
      {
        lastDay = myTMNow->tm_mday;
        rainCalendarDay = 0.0;
        startOfDayRain = rainTotal;
      }



      bool dataStale;
      dataStale = false;
      // check for stale data from WXLink


      if (WXLink_Present)
      {
        if (lastMessageID != WXMessageID)
        {
          dataStale = false;
          lastMessageID = WXMessageID;
        }
        else
        {
          dataStale = true;
        }
      }


      if ((WeatherUnderground_StationID.length() == 0) || (WeatherUnderground_StationKey.length() == 0) )
      {
        Serial.println("-----------");
        Serial.println(" WeatherUnderground Disabled");
        Serial.println("-----------");
      }
      else // Check next and the go for it.
        if ((WeatherUnderground_StationID == "XXXX") || (WeatherUnderground_StationKey == "YYYY") )
        {
          Serial.println("-----------");
          Serial.println(" WeatherUnderground Disabled");
          Serial.println("-----------");

        }
        else
        {
          Serial.println("Attempting to send data to WeatherUnderground");
          if (dataStale == false)
            Serial.println("WeatherUnderground Data New - sent");
          else
            Serial.println("WeatherUnderground Data Stale - Not sent");

          if (dataStale == false)
          {
            if (sendWeatherUndergroundData() == 0)
            {
              // Failed - try again
              sendWeatherUndergroundData();
            }


          }
        }



      delay(100);

      // send data up to PubNub

      if (pubNubEnabled == 1)
      {

        String SendString = "{\"FullDataString\": \"" + RestDataString + "\"}"; //Send the request

        // publish it

        publishPubNubMessage(SendString);
      }
    }






  } // end of 2 second loop



  yield();

}

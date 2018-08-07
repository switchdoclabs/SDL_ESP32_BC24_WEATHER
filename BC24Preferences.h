//
// Non volalile preferences functions
//


// Global preference variables

unsigned int currentLEDMode;
unsigned int OLEDOn;


unsigned int darkLight;

String WiFi_SSID = "";
String WiFi_psk = "";

int ClockTimeOffsetToUTC = 0;



void resetPreferences()
{
  preferences.begin("BC24init", false);
  preferences.clear();
  preferences.end();

  ClockTimeOffsetToUTC = DEFAULTCLOCKTIMEOFFSETTOUTC;
  Serial.println("----Clearing Preferences---");


}

void writePreferences()
{
  preferences.begin("BC24init", false);


  preferences.putInt("currentLEDMode", currentLEDMode);
  preferences.putInt("OLEDOn", OLEDOn);
  preferences.putInt("darkLight", darkLight);

  preferences.putString("WiFi_SSID", WiFi_SSID);
  preferences.putString("WiFi_psk", WiFi_psk);

  Serial.print("ClockTimeOffsetToUTC=");
  Serial.println(ClockTimeOffsetToUTC);
  preferences.putInt("COffsetToUTC", ClockTimeOffsetToUTC);

  preferences.putInt("EnglishOrMetric", EnglishOrMetric);
  preferences.putInt("WeatherDisplayMode", WeatherDisplayMode);

  preferences.putFloat("altitude_meters", altitude_meters);
  preferences.putString("adminPassword", adminPassword);

  preferences.putString("WeatherUnderground_StationID", WeatherUnderground_StationID);
  preferences.putString("WeatherUnderground_StationKey", WeatherUnderground_StationKey);

  preferences.putInt("pubNubEnabled", pubNubEnabled);
  preferences.putString("SDL2PubNubCode", SDL2PubNubCode);
  preferences.putString("SDL2PubNubCode_Sub", SDL2PubNubCode_Sub);

  preferences.putString("as3935_Params", as3935_Params);


  preferences.end();

#ifdef BC24DEBUG
  Serial.println("----Writing Preferences---");
  Serial.print("currentLEDMode="); Serial.println(currentLEDMode);
  Serial.print("OLEDOn="); Serial.println(OLEDOn);
  Serial.print("darkLight="); Serial.println(darkLight);

  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);

  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("WeatherDisplayMode:");
  Serial.println(WeatherDisplayMode);
  Serial.print("Station Name:");
  Serial.println(stationName);
  Serial.print("Altitude:");
  Serial.println(altitude_meters);
  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");
  Serial.print("WeatherUnderground Station ID:");
  Serial.println(WeatherUnderground_StationID);
  Serial.print("WeatherUnderground Station KEY:");
  Serial.println(WeatherUnderground_StationKey.substring(0, 2) + "******");

  Serial.print("pubNubEnabled=");
  Serial.println(pubNubEnabled);
  Serial.print("SDL2PubNubCode=");
  Serial.println(SDL2PubNubCode);
  Serial.print("SDL2PubNubCode_Sub=");
  Serial.println(SDL2PubNubCode_Sub);
  Serial.print("as3935_Params=");
  Serial.println(as3935_Params);
  Serial.println("--------------------------");

#endif


}

void readPreferences()
{
  preferences.begin("BC24init", false);

  currentLEDMode = preferences.getInt("currentLEDMode", 0);
  OLEDOn = preferences.getInt("OLEDOn", 1);
  darkLight = preferences.getInt("darkLight", 1);

  WiFi_SSID = preferences.getString("WiFi_SSID", "");
  WiFi_psk = preferences.getString("WiFi_psk", "");
  ClockTimeOffsetToUTC = preferences.getInt("COffsetToUTC", DEFAULTCLOCKTIMEOFFSETTOUTC);


  EnglishOrMetric = preferences.getInt("EnglishOrMetric");
  WeatherDisplayMode = preferences.getInt("WeatherDisplayMode", DISPLAY_WEATHER_LARGE);

  altitude_meters = preferences.getFloat("altitude_meters", 637.0);
  adminPassword = preferences.getString("adminPassword", "admin");

  WeatherUnderground_StationID = preferences.getString("WeatherUnderground_StationID", "");
  WeatherUnderground_StationKey = preferences.putString("WeatherUnderground_StationKey", "");

  pubNubEnabled = preferences.getInt("pubNubEnabled");
  SDL2PubNubCode = preferences.getString("SDL2PubNubCode", "XX");
  SDL2PubNubCode_Sub = preferences.getString("SDL2PubNubCode_Sub", "XX");

  as3935_Params = preferences.getString("as3935_Params", "");

  preferences.end();

#ifdef BC24DEBUG
  Serial.println("----Reading Preferences---");
  Serial.print("currentLEDMode="); Serial.println(currentLEDMode);
  Serial.print("OLEDOn="); Serial.println(OLEDOn);
  Serial.print("darkLight="); Serial.println(darkLight);

  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);

  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("WeatherDisplayMode:");
  Serial.println(WeatherDisplayMode);
  Serial.print("Station Name:");
  Serial.println(stationName);
  Serial.print("Altitude:");
  Serial.println(altitude_meters);
  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");
  Serial.print("WeatherUnderground Station ID:");
  Serial.println(WeatherUnderground_StationID);
  Serial.print("WeatherUnderground Station KEY:");
  Serial.println(WeatherUnderground_StationKey.substring(0, 2) + "******");

  Serial.print("pubNubEnabled=");
  Serial.println(pubNubEnabled);
  Serial.print("SDL2PubNubCode=");
  Serial.println(SDL2PubNubCode);
  Serial.print("SDL2PubNubCode_Sub=");
  Serial.println(SDL2PubNubCode_Sub);
  Serial.print("as3935_Params=");
  Serial.println(as3935_Params);
  Serial.println("--------------------------");
#endif
}




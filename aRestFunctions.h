//
//
//
// aRest Command functions
//
//


//
// BC24 Control functions
//



int setCurrentLEDMode(String command)
{
  Serial.println("setCurrentLEDMode");
  Serial.print("Command =");
  Serial.println(command);

  String sentPassword;
  String value;

  sentPassword = getValue(command, ',', 0);
  value = getValue(command, ',', 1);

  if (sentPassword == adminPassword)
  {

    int myValue = value.toInt();

    if (!validate_setCurrentLEDMode(myValue))
    {
      // fails - bad mode
      myValue = BC24_WEATHER_MODE_NON_IMPLEMENTED;
    }
    currentLEDMode = myValue;

    writePreferences();
    BC24CheckForClear();
    return 1;
  }
  return 0;

}


int setOLEDOn(String command)
{
  Serial.println("setOLEDOn");
  Serial.print("Command =");
  Serial.println(command);

  String sentPassword;
  String value;

  sentPassword = getValue(command, ',', 0);
  value = getValue(command, ',', 1);

  if (sentPassword == adminPassword)
  {

    int myValue = value.toInt();


    // validate, set to On if it doesn't make sense
    if (myValue == 0)
      OLEDOn = myValue;
    else if (myValue == 1)
      OLEDOn = myValue;
    else
      myValue = 1;


    writePreferences();
    return 1;
  }
  return 0;
}

int setClockTimeOffsetToUTC(String command)
{
  Serial.println("ClockTimeOffsetToUTC");
  Serial.print("Command =");
  Serial.println(command);

  String sentPassword;
  String value;

  sentPassword = getValue(command, ',', 0);
  value = getValue(command, ',', 1);

  if (sentPassword == adminPassword)
  {

    int myValue = value.toInt();

    ClockTimeOffsetToUTC = myValue;

    timeClient.setTimeOffset(ClockTimeOffsetToUTC);
    timeClient.forceUpdate();
    time_t t;
    t = timeClient.getEpochTime();
    setTime(t);


    writePreferences();
    return 1;
  }
  return 0;
}


int setDarkLight(String command)
{
  Serial.println("setDarkLight");
  Serial.print("Command =");
  Serial.println(command);

  String sentPassword;
  String value;

  sentPassword = getValue(command, ',', 0);
  value = getValue(command, ',', 1);

  if (sentPassword == adminPassword)
  {

    int myValue = value.toInt();


    // validate, set to On if it doesn't make sense
    if (myValue == 0)
      darkLight = myValue;
    else if (myValue == 1)
      darkLight = myValue;
    else
      myValue = 1;

    setUpDarkLight();
    writePreferences();
    return 1;
  }
  return 0;
}

// other functions
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  if (state == 0)  // ESP8266 inverts sense (HIGH = off, LOW = ON)
    state = 1;
  else
    state = 0;

  digitalWrite(blinkPin, state);
  return 1;
}

int setWeatherPlusIDControl(String command) {



  rest.set_id(command);
  return 1;
}

int resetWiFiAccessPoint(String command)
{
  Serial.print("Command =");
  Serial.println(command);




  if (command == adminPassword)
  {
    WiFi_psk = "XXX";
    WiFi_SSID = "XXX";
    writePreferences();
    return 1;
  }
  else
    return 0;


  //http://192.168.1.134/resetWiFiAccessPoint?params=adminpassword
  return 1;


}

int resetBC24Weather(String command) {

  Serial.println("resetBC24Weather - settings invalidated");
  Serial.print("Command =");
  Serial.println(command);
  if (command == adminPassword)
  {

    resetPreferences();

    system_restart();

    // qdelay(10000);

    return 1;
  }
  return 0;
}

int setAdminPassword(String command)
{
  Serial.print("Command =");
  Serial.println(command);


  String oldPassword;
  String neWiFi_psk;

  oldPassword = getValue(command, ',', 0);
  neWiFi_psk = getValue(command, ',', 1);

  if (oldPassword == adminPassword)
  {
    adminPassword = neWiFi_psk;
    writePreferences();
    return 1;
  }
  else
    return 0;


  //http://192.168.1.134/setAdminPassword?params=oldpassword,neWiFi_psk
  return 1;

}

int setWUSID(String command)
{
  Serial.print("Command =");
  Serial.println(command);


  String sentPassword;
  String WUSID;

  sentPassword = getValue(command, ',', 0);
  WUSID = getValue(command, ',', 1);

  Serial.print("adminPassword=");
  Serial.println(adminPassword);
  Serial.print("sentPassword=");
  Serial.println(sentPassword);

  if (sentPassword == adminPassword)
  {
    WeatherUnderground_StationID = WUSID;
    writePreferences();
    return 1;
  }
  else
    return 0;


  return 1;

}

int setWUKEY(String command)
{
  Serial.print("Command =");
  Serial.println(command);


  String sentPassword;
  String WUKEY;

  sentPassword = getValue(command, ',', 0);
  WUKEY = getValue(command, ',', 1);

  if (sentPassword == adminPassword)
  {
    WeatherUnderground_StationKey = WUKEY;
    writePreferences();
    return 1;
  }
  else
    return 0;


  return 1;

}


int rebootBC24Weather(String command) {



  system_restart();
  // ESP.reset();
  delay(10000);

  return 1;
}

int setDateTime(String command)
{

  String _time;
  String _date;
  String _password;

  _password = getValue(command, ',', 0);
  _date = getValue(command, ',', 1);
  _time = getValue(command, ',', 2);

  Serial.println("command values:");
  Serial.println(_password);
  Serial.println(_date);
  Serial.println(_time);

  _date.replace("%20", " ");
  Serial.println(_password);
  Serial.println(_date);
  Serial.println(_time);

  if (_password == adminPassword)
  {
    if ((_time.length() > 0) && (_date.length() > 0))
    {
      //RtcDateTime compiled = RtcDateTime(_date.c_str(), _time.c_str());
      Serial.println("Updating DateTime in RTC");

    }
    else
    {
      Serial.println("Not updating DateTime in RTC");
      return 2;
    }
    return 1;
  }
  else
    return 0;
}


/*
  // FOTA update commands


  int updateBC24Weather(String command)
  {

  Serial.println("updateBC24Weather");
  Serial.print("Command =");
  Serial.println(command);
  if (command == adminPassword)
  {
    updateDisplay(DISPLAY_UPDATING);
    delay(5000);

    updateDisplay(DISPLAY_UPDATE_FINISHED);
    t_httpUpdate_return ret = ESPhttpUpdate.update("www.switchdoc.com", 80, "/BC24WeatherUpdater.php", WEATHERPLUSESP8266VERSION);
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.println("[update] Update failed.");
        updateDisplay(DISPLAY_NO_UPDATE_FAILED);
        delay(5000);

        return 1;

        break;
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Updates.");
        updateDisplay(DISPLAY_NO_UPDATE_AVAILABLE);
        delay(5000);
        return 2;
        break;
      case HTTP_UPDATE_OK:

        Serial.println("[update] Update ok."); // may not called we reboot the ESP

        return 3;
        break;
    }



  }
  return 0;


  }
*/


// connection commands

int resetToDefaults(String command)
{


  return 1;

}

int enableCWOPControl(String command) {

  // Get state from command
  int state = command.toInt();


  return 1;
}

int enableTwitterControl(String command) {

  // Get state from command
  int state = command.toInt();

  return 1;
}


// Weather Controls

int weatherSmallControl(String command) {

  WeatherDisplayMode = DISPLAY_WEATHER_SMALL;
  writePreferences();

  return 1;
}


int weatherMediumControl(String command) {

  WeatherDisplayMode = DISPLAY_WEATHER_MEDIUM;
  writePreferences();
  return 1;
}



int weatherLargeControl(String command) {

  WeatherDisplayMode = DISPLAY_WEATHER_LARGE;
  writePreferences();
  return 1;
}



int weatherDemoControl(String command) {

  WeatherDisplayMode = DISPLAY_WEATHER_DEMO;
  writePreferences();
  return 1;
}



int englishUnitControl(String command) {

  EnglishOrMetric = 0;
  writePreferences();
  return 1;
}


int metricUnitControl(String command) {

  EnglishOrMetric = 1;
  writePreferences();
  return 1;
}





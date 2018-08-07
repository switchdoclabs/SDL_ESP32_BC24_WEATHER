//
// Functions for BC24 WEATHER LED Display
//


NeoFire fire;

//  LED Modes

#define BC24_WEATHER_MODE_OUTSIDE_TEMPERATURE 0
#define BC24_WEATHER_MODE_WINDSPEED_DIRECTION 1
#define BC24_WEATHER_MODE_OUTSIDE_HUMIDITY    2
#define BC24_WEATHER_MODE_INSIDE_TEMPERATURE  3
#define BC24_WEATHER_MODE_INSIDE_HUMIDITY     4
#define BC24_WEATHER_MODE_BP_TREND            5
#define BC24_WEATHER_MODE_LIGHTNING           6
#define BC24_WEATHER_MODE_AIRQUALITY          7


#define BC24_WEATHER_MODE_NO_ROTATE_BLANK     20
#define BC24_WEATHER_MODE_NO_ROTATE_RAINBOW   21
#define BC24_WEATHER_MODE_NO_ROTATE_DOWJONES  22
#define BC24_WEATHER_MODE_NO_ROTATE_FIRE      23
#define BC24_WEATHER_MODE_NO_ROTATE_CLOCK     24

#define BC24_WEATHER_MODE_NON_IMPLEMENTED     100



bool validate_setCurrentLEDMode(int newMode)
{

  switch (newMode)
  {

    case BC24_WEATHER_MODE_OUTSIDE_TEMPERATURE:
    case BC24_WEATHER_MODE_WINDSPEED_DIRECTION:
    case BC24_WEATHER_MODE_OUTSIDE_HUMIDITY:
    case BC24_WEATHER_MODE_INSIDE_TEMPERATURE:
    case BC24_WEATHER_MODE_INSIDE_HUMIDITY:
    case BC24_WEATHER_MODE_BP_TREND:

    case BC24_WEATHER_MODE_LIGHTNING:
    case BC24_WEATHER_MODE_AIRQUALITY:

    case BC24_WEATHER_MODE_NO_ROTATE_CLOCK:
    case BC24_WEATHER_MODE_NO_ROTATE_BLANK:
    case BC24_WEATHER_MODE_NO_ROTATE_RAINBOW:
    case BC24_WEATHER_MODE_NO_ROTATE_DOWJONES:
    case BC24_WEATHER_MODE_NO_ROTATE_FIRE:
      return true;
      break;
    default:
      return false;

  }

}

void BC24CheckForClear()
{
  switch (currentLEDMode)
  {
    case BC24_WEATHER_MODE_NO_ROTATE_BLANK:
    case BC24_WEATHER_MODE_NO_ROTATE_CLOCK:
    case BC24_WEATHER_MODE_OUTSIDE_HUMIDITY:
    case BC24_WEATHER_MODE_INSIDE_HUMIDITY:
    case BC24_WEATHER_MODE_NO_ROTATE_FIRE:
    case BC24_WEATHER_MODE_NO_ROTATE_RAINBOW:
    case BC24_WEATHER_MODE_OUTSIDE_TEMPERATURE:
    case BC24_WEATHER_MODE_INSIDE_TEMPERATURE:
    case BC24_WEATHER_MODE_WINDSPEED_DIRECTION:
      BC24setStrip(Black);

      break;






    case BC24_WEATHER_MODE_BP_TREND:

    case BC24_WEATHER_MODE_LIGHTNING:
    case BC24_WEATHER_MODE_AIRQUALITY:



    case BC24_WEATHER_MODE_NO_ROTATE_DOWJONES:


      break;
    default:
      break;

  }


}

void update_BC24_Display()
{

  // turn off all tasks
  xSemaphoreTake( xSemaphoreClock, 10);   // Turn off Clock
  xSemaphoreTake( xSemaphoreFire, 10);
  xSemaphoreTake( xSemaphoreRainbow, 10);
  xSemaphoreTake( xSemaphoreWind, 10);

  switch (currentLEDMode)
  {

    case BC24_WEATHER_MODE_NO_ROTATE_CLOCK:

      xSemaphoreGive( xSemaphoreClock);   // Turn on Clock

      break;

    case BC24_WEATHER_MODE_OUTSIDE_HUMIDITY:
      RightCircle(AM2315_Humidity , 0.0, 100.0, Green, true, "OutHumid");
      break;

    case BC24_WEATHER_MODE_INSIDE_HUMIDITY:
      RightCircle(HDC1080_Inside_Humidity , 0.0, 100.0, Green, true, "InHumid");
      break;

    case BC24_WEATHER_MODE_NO_ROTATE_FIRE:
      xSemaphoreGive( xSemaphoreFire);   // Turn on Fire
      break;

    case BC24_WEATHER_MODE_NO_ROTATE_RAINBOW:
      xSemaphoreGive( xSemaphoreRainbow);   // Turn on Rainbow
      break;

    case BC24_WEATHER_MODE_WINDSPEED_DIRECTION:
      xSemaphoreGive( xSemaphoreWind);   // Turn on Rainbow
      break;

    case BC24_WEATHER_MODE_OUTSIDE_TEMPERATURE:

      ThreeRightCircle(AM2315_Temperature, -6.0, 35.0,  true, "OutTemp", 18, Blue, 29, Green, Red);
      break;


    case BC24_WEATHER_MODE_INSIDE_TEMPERATURE:

    case BC24_WEATHER_MODE_BP_TREND:

    case BC24_WEATHER_MODE_LIGHTNING:
    case BC24_WEATHER_MODE_AIRQUALITY:



    case BC24_WEATHER_MODE_NO_ROTATE_DOWJONES:


      //BC24_WEATHER_MODE_NON_IMPLEMENTED

      BC24setStrip(DarkBlue);
      break;

    case BC24_WEATHER_MODE_NO_ROTATE_BLANK:
      break;

    default:
      //BC24_WEATHER_MODE_NON_IMPLEMENTED

      BC24setStrip(DarkBlue);
      break;

  }

}



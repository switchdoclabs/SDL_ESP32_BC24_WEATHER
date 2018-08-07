// Utility function


String returnDateTime(time_t myTime)
{

char buff[40];

struct tm *myTm;
myTm = localtime(&myTime);
strftime(buff, 20, "%Y-%m-%d %H:%M:%S", myTm);
String returnString;
returnString = String(buff);

return returnString;

}


void blinkLED(int timesToBlink, int delayBetweenBlinks)
{

  int i;
  for (i = 0; i < timesToBlink; i++)
  {
    digitalWrite(BLINKPIN, 0);
    vTaskDelay((delayBetweenBlinks / 2) / portTICK_PERIOD_MS);

    digitalWrite(BLINKPIN, 1);
    vTaskDelay((delayBetweenBlinks / 2) / portTICK_PERIOD_MS);
  }

}
#include <rom/rtc.h>

void print_reset_reason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1 : Serial.println ("POWERON_RESET"); break;         /**<1, Vbat power on reset*/
    case 3 : Serial.println ("SW_RESET"); break;              /**<3, Software reset digital core*/
    case 4 : Serial.println ("OWDT_RESET"); break;            /**<4, Legacy watch dog reset digital core*/
    case 5 : Serial.println ("DEEPSLEEP_RESET"); break;       /**<5, Deep Sleep reset digital core*/
    case 6 : Serial.println ("SDIO_RESET"); break;            /**<6, Reset by SLC module, reset digital core*/
    case 7 : Serial.println ("TG0WDT_SYS_RESET"); break;      /**<7, Timer Group0 Watch dog reset digital core*/
    case 8 : Serial.println ("TG1WDT_SYS_RESET"); break;      /**<8, Timer Group1 Watch dog reset digital core*/
    case 9 : Serial.println ("RTCWDT_SYS_RESET"); break;      /**<9, RTC Watch dog Reset digital core*/
    case 10 : Serial.println ("INTRUSION_RESET"); break;      /**<10, Instrusion tested to reset CPU*/
    case 11 : Serial.println ("TGWDT_CPU_RESET"); break;      /**<11, Time Group reset CPU*/
    case 12 : Serial.println ("SW_CPU_RESET"); break;         /**<12, Software reset CPU*/
    case 13 : Serial.println ("RTCWDT_CPU_RESET"); break;     /**<13, RTC Watch dog Reset CPU*/
    case 14 : Serial.println ("EXT_CPU_RESET"); break;        /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : Serial.println ("RTCWDT_BROWN_OUT_RESET"); break; /**<15, Reset when the vdd voltage is not stable*/
    case 16 : Serial.println ("RTCWDT_RTC_RESET"); break;     /**<16, RTC Watch dog reset digital core and rtc module*/
    default : Serial.println ("NO_MEAN");
  }
}


void SetRandomSeed()
{
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  vTaskDelay(1 / portTICK_PERIOD_MS);

  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }

  // Serial.println(seed);
  randomSeed(seed);
}


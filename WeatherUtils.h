// Weather Utilities


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


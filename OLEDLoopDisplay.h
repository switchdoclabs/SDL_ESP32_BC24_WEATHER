//
// Main weather display loop
//


void MainWeatherOLEDLoopDisplay()
{

  if (OLEDOn)
  {



    Serial.println("--------->Entering Display Loop");


    updateDisplay(WeatherDisplayMode);


#ifdef OLEDEXPERIMENT
    xSemaphoreGive( xSemaphoreOLEDLoopUpdate);   // release semaphore to take sample if needed
    xSemaphoreTake( xSemaphoreOLEDLoopUpdate, portMAX_DELAY );// grab it again
#endif

    if (AS3935Present == true)
    {
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      updateDisplay(DISPLAY_LIGHTNING_STATUS);
      vTaskDelay(3000 / portTICK_PERIOD_MS);;
    }
#ifdef OLEDEXPERIMENT
    xSemaphoreGive( xSemaphoreOLEDLoopUpdate);   // release semaphore to take sample if needed
    xSemaphoreTake( xSemaphoreOLEDLoopUpdate, portMAX_DELAY );// grab it again
#endif
    if (SunAirPlus_Present)
    {

      vTaskDelay(3000 / portTICK_PERIOD_MS);
      updateDisplay(DISPLAY_SUNAIRPLUS);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
#ifdef OLEDEXPERIMENT
    xSemaphoreGive( xSemaphoreOLEDLoopUpdate);   // release semaphore to take sample if needed
    xSemaphoreTake( xSemaphoreOLEDLoopUpdate, portMAX_DELAY );// grab it again
#endif

    if (WXLink_Present)
    {

      vTaskDelay(3000 / portTICK_PERIOD_MS);
      updateDisplay(DISPLAY_WXLINK);
      vTaskDelay(3000 / portTICK_PERIOD_MS);


      // check to see if pin 5 is stuck high (SCL is at 0) - then we are hung.
      /*
            int SCL, SDA;

            SCL = digitalRead(4);
            SDA = digitalRead(5);
            Serial.print("SCL/SDA=");
            Serial.print(SCL);
            Serial.print("/");
            Serial.println(SDA);

            if ((SCL == 0) || (SDA == 0))
            {
              resetWXLink();

            }
      */
    }



    Serial.println("<-----------OutOfDisplay");
  }
else
{
  Serial.println("OLED Turned Off");
}
}


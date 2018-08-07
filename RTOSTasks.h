// RTOS Tasks for various modes and periphials



//
//
// Debounced button task
//
//


#define DEBOUNCETIME 30

volatile int numberOfButtonInterrupts = 0;
volatile bool lastState;
volatile uint32_t debounceTimeout = 0;

// For setting up critical sections (enableinterrupts and disableinterrupts not available)
// used to disable and interrupt interrupts

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


// Interrupt Service Routine - Keep it short!
void IRAM_ATTR handleButtonInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  numberOfButtonInterrupts++;
  lastState = digitalRead(BUTTONPIN);
  debounceTimeout = xTaskGetTickCount(); //version of millis() that works from interrupt
  portEXIT_CRITICAL_ISR(&mux);
}

//
// RTOS Task for reading button pushes (debounced)
//

void taskButtonRead( void * parameter)
{
  String taskMessage = "Debounced ButtonRead Task running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
  Serial.println(taskMessage);

  // set up button Pin
  pinMode (BUTTONPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), handleButtonInterrupt, FALLING);

  uint32_t saveDebounceTimeout;
  bool saveLastState;
  int save;

  // Enter RTOS Task Loop
  for (;;)  {

    portENTER_CRITICAL_ISR(&mux); // so that value of numberOfButtonInterrupts,l astState are atomic - Critical Section
    save  = numberOfButtonInterrupts;
    saveDebounceTimeout = debounceTimeout;
    saveLastState  = lastState;
    portEXIT_CRITICAL_ISR(&mux); // end of Critical Section

    bool currentState = digitalRead(BUTTONPIN);

    // This is the critical IF statement
    // if Interrupt Has triggered AND Button Pin is in same state AND the debounce time has expired THEN you have the button push!
    //
    if ((save != 0) //interrupt has triggered
        && (currentState == saveLastState) // pin is still in the same state as when intr triggered
        && (millis() - saveDebounceTimeout > DEBOUNCETIME ))
    { // and it has been low for at least DEBOUNCETIME, then valid keypress

#ifdef BC24DEBUG
      if (currentState == LOW)
      {
        Serial.printf("Button is pressed and debounced, current tick = % d\n", millis());
      }
      else
      {
        Serial.printf("Button is released and debounced, current tick = % d\n", millis());
      }


      Serial.printf("Button Interrupt Triggered % d times, current State = % u, time since last trigger % dms\n", save, currentState, millis() - saveDebounceTimeout);
#endif

      portENTER_CRITICAL_ISR(&mux); // can't change it unless, atomic - Critical section
      numberOfButtonInterrupts = 0; // acknowledge keypress and reset interrupt counter
      portEXIT_CRITICAL_ISR(&mux);

      //currentLEDMode = (currentLEDMode + 1) % BC24NUMBEROFMODES;
      writePreferences();


      //sendcurrentLEDMode();

      //Serial.print("currentLEDMode =");
      //Serial.println(currentLEDModeToString(currentLEDMode));

      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


void taskSingleBlink( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreSingleBlink ) > 0)
    {

      BC24OneBlink(Blue, 300);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

  }

}


void taskClock( void * parameter)
{


  pixelColor_t myStatusColor;

  for ( ;; )
  {
    if (uxSemaphoreGetCount( xSemaphoreClock ) > 0)
    {
      if (WiFiPresent)
      {
        int currentPixel = 0;
#ifdef BC24DEBUG
        Serial.println("Clock task running");
#endif
        time_t currentTime;
        currentTime = timeClient.getEpochTime();
        // Serial.println("After epoch time");



        tmElements_t tm_currentTime;
        breakTime(currentTime, tm_currentTime);
        Serial.println("After breaktime");
        digitalClockDisplay();

        displayClock(tm_currentTime.Hour, tm_currentTime.Minute, tm_currentTime.Second);
        // Serial.println("After displayClock");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // Serial.println("after vTaskDealy");
      }

      else
      {
        // no Wifi


        BC24BottomFiveBlink(Red, 1000);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
      }

    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }


}


void taskUpdate( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreUpdate ) > 0)
    {

      update_BC24_Display();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }

}



void taskFire( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreFire ) > 0)
    {

      fire.Draw();
      vTaskDelay(random(50, 150) / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


  }

}


void taskWind( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreWind ) > 0)
    {

      ThreeRightCirclePlusDirection(currentWindSpeed, 0.0, 40.0,  false, currentWindDirection, "Wind", 18, Green, 35, Green, Red);

      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }


  }

}


#ifdef OLEDEXPERIMENT
void taskOLEDLoopUpdate( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreOLEDLoopUpdate ) > 0)
    {


      MainWeatherOLEDLoopDisplay();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }


  }

}
#endif

void taskRESTCommand( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    if (uxSemaphoreGetCount( xSemaphoreRESTCommand ) > 0)
    {


      // Handle REST calls
      WiFiClient client = server.available();

      int timeout;
      timeout = 0;
      if (client)
      {

        // Thank you to MAKA69 for this suggestion
        while (!client.available()) {
          Serial.print(".");
          vTaskDelay(10 / portTICK_PERIOD_MS);
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

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }


  }

}

void taskRainbow( void * parameter)
{
  // Enter RTOS Task Loop
  for (;;)  {

    strand_t * pStrand = &STRANDS[0];
    if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
    {
      while (1)
      {
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {

          rainbow(pStrand, 0, 2000);
        }
        else
          break;
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {

          scanner(pStrand, 0, 2000);
        }
        else
          break;

        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          scanner(pStrand, 1, 2000);
        }
        else
          break;
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          scanner(pStrand, 0, 2000);
        }
        else
          break;
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          rainbow(pStrand, 0, 2000);
        }
        else
          break;

        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          scanner(pStrand, 1, 2000); // A tiny delay can smooth things out
        }
        else
          break;
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          rainbow(pStrand, 0, 2000);
        }
        else
          break;
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          scanner(pStrand, 5, 2000);
        }
        else
          break;
        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          rainbow(pStrand, 0, 2000);
        }
        else
          break;

        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          rainbow(pStrand, 0, 2000);
        }
        else
          break;

        if (uxSemaphoreGetCount( xSemaphoreRainbow ) > 0)
        {
          rainbow(pStrand, 0, 2000);
        }
        else
          break;

      }

    }
    else
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

  }
}



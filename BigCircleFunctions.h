




#include "esp32_digital_led_lib.h"



// standard colors

//pixelColor_t pixelFromRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w)

const pixelColor_t BrightRed = pixelFromRGBW(255, 0, 0, 0);
const pixelColor_t BrightGreen = pixelFromRGBW(0, 255, 0, 0);
const pixelColor_t BrightBlue = pixelFromRGBW(0, 0, 255, 0);
const pixelColor_t BrightWhite = pixelFromRGBW(0, 0, 0, 255);

// these four are for the lightDark variable to change
pixelColor_t Red = pixelFromRGBW(255, 0, 0, 0);
pixelColor_t Green = pixelFromRGBW(0, 255, 0, 0);
pixelColor_t Blue = pixelFromRGBW(0, 0, 255, 0);
pixelColor_t White = pixelFromRGBW(0, 0, 0, 255);

const pixelColor_t BrightYellow = pixelFromRGBW(255, 255, 0, 0);
const pixelColor_t BrightCyan = pixelFromRGBW(0, 255, 255, 0);
const pixelColor_t BrightPurple = pixelFromRGBW(255, 0, 255, 0);

const pixelColor_t DarkRed = pixelFromRGBW(32, 0, 0, 0);
const pixelColor_t DarkGreen = pixelFromRGBW(0, 32, 0, 0);
const pixelColor_t DarkBlue = pixelFromRGBW(0, 0, 32, 0);
const pixelColor_t DarkWhite = pixelFromRGBW(0, 0, 0, 32);

const pixelColor_t DarkYellow = pixelFromRGBW(32, 32, 0, 0);
const pixelColor_t DarkCyan = pixelFromRGBW(0, 32, 32, 0);
const pixelColor_t DarkPurple = pixelFromRGBW(32, 0, 32, 0);


const pixelColor_t Black = pixelFromRGBW(0, 0, 0, 0);

// strands for our system

strand_t STRANDS[] = { // Avoid using any of the strapping pins on the ESP32
  { .rmtChannel = 1, .gpioNum = 21, .ledType = LED_SK6812W_V1, .brightLimit = 128, .numPixels =  24,
    .pixels = nullptr, ._stateVars = nullptr
  },
  //{ .rmtChannel = 2, .gpioNum = 18, .ledType = LED_WS2812B_V3, .brightLimit = 32, .numPixels =  93,
  //  .pixels = nullptr, ._stateVars = nullptr
  // },
  // {.rmtChannel = 3, .gpioNum = 19, .ledType = LED_WS2812B_V3, .brightLimit = 32, .numPixels =  64,
  //  .pixels = nullptr, ._stateVars = nullptr},
  //{.rmtChannel = 0, .gpioNum = 16, .ledType = LED_WS2812B_V3, .brightLimit = 32, .numPixels = 256,
  // .pixels = nullptr, ._stateVars = nullptr},
  //  {.rmtChannel = 0, .gpioNum = 16, .ledType = LED_SK6812W_V1, .brightLimit = 32, .numPixels = 300,
  //   .pixels = nullptr, ._stateVars = nullptr},
  //{ .rmtChannel = 0, .gpioNum = 16, .ledType = LED_SK6812W_V1, .brightLimit = 32, .numPixels = 24,
  // .pixels = nullptr, ._stateVars = nullptr
  //},
};

//strand_t * strands [] = { &STRANDS[0], &STRANDS[1], &STRANDS[2], &STRANDS[3] };
strand_t * strands [] = { &STRANDS[0] };
int STRANDCNT = sizeof(STRANDS) / sizeof(STRANDS[0]);

// Forward declarations
void rainbow(strand_t *, unsigned long, unsigned long);
void scanner(strand_t *, unsigned long, unsigned long);
void dumpDebugBuffer(int, char *);


#if DEBUG_ESP32_DIGITAL_LED_LIB
int digitalLeds_debugBufferSz = 1024;
char * digitalLeds_debugBuffer = static_cast<char*>(calloc(digitalLeds_debugBufferSz, sizeof(char)));
#endif

void gpioSetup(int gpioNum, int gpioMode, int gpioVal) {
#if defined(ARDUINO) && ARDUINO >= 100
  pinMode (gpioNum, gpioMode);
  digitalWrite (gpioNum, gpioVal);
#elif defined(ESP_PLATFORM)
  gpio_num_t gpioNumNative = static_cast<gpio_num_t>(gpioNum);
  gpio_mode_t gpioModeNative = static_cast<gpio_mode_t>(gpioMode);
  gpio_pad_select_gpio(gpioNumNative);
  gpio_set_direction(gpioNumNative, gpioModeNative);
  gpio_set_level(gpioNumNative, gpioVal);
#endif
}


#define min(X, Y) (((X)<(Y))?(X):(Y))

//**************************************************************************//
int getMaxMalloc(int min_mem, int max_mem) {
  int prev_size = min_mem;
  int curr_size = min_mem;
  int max_free = 0;
  //  Serial.print("checkmem: testing alloc from ");
  //  Serial.print(min_mem);
  //  Serial.print(" : ");
  //  Serial.print(max_mem);
  //  Serial.println(" bytes");
  while (1) {
    void * foo1 = malloc(curr_size);
    //    Serial.print("checkmem: attempt alloc of ");
    //    Serial.print(curr_size);
    //    Serial.print(" bytes --> pointer 0x");
    //    Serial.println((uintptr_t)foo1, HEX);
    if (foo1 == nullptr) {  // Back off
      max_mem = min(curr_size, max_mem);
      //      Serial.print("checkmem: backoff 2 prev = ");
      //      Serial.print(prev_size);
      //      Serial.print(", curr = ");
      //      Serial.print(curr_size);
      //      Serial.print(", max_mem = ");
      //      Serial.print(max_mem);
      //      Serial.println();
      curr_size = (int)(curr_size - (curr_size - prev_size) / 2.0);
      //      Serial.print("checkmem: backoff 2 prev = ");
      //      Serial.print(prev_size);
      //      Serial.print(", curr = ");
      //      Serial.print(curr_size);
      //      Serial.println();
    }
    else {  // Advance
      free(foo1);
      max_free = curr_size;
      prev_size = curr_size;
      curr_size =  min(curr_size * 2, max_mem);
      //      Serial.print("checkmem: advance 2 prev = ");
      //      Serial.print(prev_size);
      //      Serial.print(", curr = ");
      //      Serial.print(curr_size);
      //      Serial.println();
    }
    if (abs(curr_size - prev_size) == 0) {
      break;
    }
  }
  Serial.print("checkmem: max free heap = ");
  Serial.print(esp_get_free_heap_size());
  Serial.print(" bytes, max allocable = ");
  Serial.print(max_free);
  Serial.println(" bytes");
  return max_free;
}

void dumpSysInfo() {
  esp_chip_info_t sysinfo;
  esp_chip_info(&sysinfo);
  Serial.print("Model: ");
  Serial.print((int)sysinfo.model);
  Serial.print("; Features: 0x");
  Serial.print((int)sysinfo.features, HEX);
  Serial.print("; Cores: ");
  Serial.print((int)sysinfo.cores);
  Serial.print("; Revision: r");
  Serial.println((int)sysinfo.revision);
}

void dumpDebugBuffer(int id, char * debugBuffer)
{
  Serial.print("DEBUG: (");
  Serial.print(id);
  Serial.print(") ");
  Serial.println(debugBuffer);
  debugBuffer[0] = 0;
}


//**************************************************************************//
class Scannerer {
  private:
    strand_t * pStrand;
    pixelColor_t minColor;
    pixelColor_t maxColor;
    int prevIdx;
    int currIdx;
  public:
    Scannerer(strand_t *, pixelColor_t);
    void drawNext();
};

Scannerer::Scannerer(strand_t * pStrandIn, pixelColor_t maxColorIn)
{
  pStrand = pStrandIn;
  minColor = pixelFromRGBW(0, 0, 0, 0);
  maxColor = maxColorIn;
  prevIdx = 0;
  currIdx = 0;
}

void Scannerer::drawNext()
{
  pStrand->pixels[prevIdx] = minColor;
  pStrand->pixels[currIdx] = maxColor;
  digitalLeds_updatePixels(pStrand);
  prevIdx = currIdx;
  currIdx++;
  if (currIdx >= pStrand->numPixels) {
    currIdx = 0;
  }
}

void scanners(strand_t * strands[], int numStrands, unsigned long delay_ms, unsigned long timeout_ms)
{
  //Scannerer scan(pStrand); Scannerer * pScanner = &scan;
  Scannerer * pScanner[numStrands];
  int i;
  uint8_t c = strands[0]->brightLimit; // TODO: improve
  pixelColor_t scanColors [] = {
    pixelFromRGBW(c, 0, 0, 0),
    pixelFromRGBW(0, c, 0, 0),
    pixelFromRGBW(c, c, 0, 0),
    pixelFromRGBW(0, 0, c, 0),
    pixelFromRGBW(c, 0, c, 0),
    pixelFromRGBW(0, c, c, 0),
    pixelFromRGBW(c, c, c, 0),
    pixelFromRGBW(0, 0, 0, c),
  };


  int colors = random(7);
#ifdef BC24DBUG
  Serial.print("DEMO: scanners(");
#endif
  for (i = 0; i < numStrands; i++) {
    pScanner[i] = new Scannerer(strands[i], scanColors[colors]);
#ifdef BC24DEBUG
    if (i > 0) {
      Serial.print(", ");
    }
    Serial.print("ch");
    Serial.print(strands[i]->rmtChannel);
    Serial.print(" (0x");
    Serial.print((uint32_t)pScanner[i], HEX);
    Serial.print(")");
    Serial.print(" #");
    Serial.print((uint32_t)scanColors[i].num, HEX);
#endif
  }
#ifdef BC24DEBUG

  Serial.print(")");
  Serial.println();
#endif
  unsigned long start_ms = millis();
  while (timeout_ms == 0 || (millis() - start_ms < timeout_ms)) {
    for (i = 0; i < numStrands; i++) {
      pScanner[i]->drawNext();
    }
    delay(delay_ms);
  }
  for (i = 0; i < numStrands; i++) {
    delete pScanner[i];
    digitalLeds_resetPixels(strands[i]);
  }
}

void scanner(strand_t * pStrand, unsigned long delay_ms, unsigned long timeout_ms)
{
  strand_t * strands [] = { pStrand };
  scanners(strands, 1, delay_ms, timeout_ms);
}

//**************************************************************************//
class Rainbower {
  private:
    strand_t * pStrand;
    const uint8_t color_div = 4;
    const uint8_t anim_step = 1;
    uint8_t anim_max;
    uint8_t stepVal1;
    uint8_t stepVal2;
    pixelColor_t color1;
    pixelColor_t color2;
  public:
    Rainbower(strand_t *);
    void drawNext();
};

Rainbower::Rainbower(strand_t * pStrandIn)
{
  pStrand = pStrandIn;
  anim_max = pStrand->brightLimit - anim_step;
  stepVal1 = 0;
  stepVal2 = 0;
  color1 = pixelFromRGBW(anim_max, 0, 0, 0);
  color2 = pixelFromRGBW(anim_max, 0, 0, 0);
}

void Rainbower::drawNext()
{
  color1 = color2;
  stepVal1 = stepVal2;
  for (uint16_t i = 0; i < pStrand->numPixels; i++) {
    pStrand->pixels[i] = pixelFromRGBW(color1.r / color_div, color1.g / color_div, color1.b / color_div, 0);
    if (i == 1) {
      color2 = color1;
      stepVal2 = stepVal1;
    }
    switch (stepVal1) {
      case 0:
        color1.g += anim_step;
        if (color1.g >= anim_max)
          stepVal1++;
        break;
      case 1:
        color1.r -= anim_step;
        if (color1.r == 0)
          stepVal1++;
        break;
      case 2:
        color1.b += anim_step;
        if (color1.b >= anim_max)
          stepVal1++;
        break;
      case 3:
        color1.g -= anim_step;
        if (color1.g == 0)
          stepVal1++;
        break;
      case 4:
        color1.r += anim_step;
        if (color1.r >= anim_max)
          stepVal1++;
        break;
      case 5:
        color1.b -= anim_step;
        if (color1.b == 0)
          stepVal1 = 0;
        break;
    }
  }
  digitalLeds_updatePixels(pStrand);
}

void rainbows(strand_t * strands[], int numStrands, unsigned long delay_ms, unsigned long timeout_ms)
{
  //Rainbower rbow(pStrand); Rainbower * pRbow = &rbow;
  Rainbower * pRbow[numStrands];
  int i;
#ifdef BC24DEBUG
  Serial.print("DEMO: rainbows(");
#endif
  for (i = 0; i < numStrands; i++) {
    pRbow[i] = new Rainbower(strands[i]);
#ifdef BC24DEBUG
    if (i > 0) {
      Serial.print(", ");
    }
    Serial.print("ch");
    Serial.print(strands[i]->rmtChannel);
    Serial.print(" (0x");
    Serial.print((uint32_t)pRbow[i], HEX);
    Serial.print(")");
#endif
  }
#ifdef BC24DEBUG
  Serial.print(")");
  Serial.println();
#endif
  unsigned long start_ms = millis();
  while (timeout_ms == 0 || (millis() - start_ms < timeout_ms)) {
    for (i = 0; i < numStrands; i++) {
      pRbow[i]->drawNext();
    }
    delay(delay_ms);
  }
  for (i = 0; i < numStrands; i++) {
    delete pRbow[i];
    digitalLeds_resetPixels(strands[i]);
  }
}

void rainbow(strand_t * pStrand, unsigned long delay_ms, unsigned long timeout_ms)
{
  strand_t * strands [] = { pStrand };
  rainbows(strands, 1, delay_ms, timeout_ms);
}

//**************************************************************************//
//  // print tests
//  Serial.println(0xFFFFFFFF, DEC);
//  Serial.println(0xFFFFFFFF, HEX);
//  Serial.println(0xFFFFFFFF, BIN);
//  Serial.println(0x7FFFFFFF, DEC);
//  Serial.println(0x7FFFFFFF, HEX);
//  Serial.println(0x7FFFFFFF, BIN);
//  Serial.println(0x00000000, DEC);
//  Serial.println(0x00000000, HEX);
//  Serial.println(0x00000000, BIN);
//  Serial.println(        -1, DEC);
//  Serial.println(        -1, HEX);
//  Serial.println(        -1, BIN);

//**************************************************************************//



void BC24clearStrip(strand_t * pStrand)
{

  digitalLeds_resetPixels(pStrand);

}

void BC24setStrip(pixelColor_t myColor )
{
  strand_t * pStrand = &STRANDS[0];



  for (int i = 0; i < pStrand->numPixels; i ++)
  {
    pStrand->pixels[i] = myColor;
    digitalLeds_updatePixels(pStrand);

  }

}


void BC24writecurrentLEDMode()
{
  strand_t * pStrand = &STRANDS[0];

  BC24clearStrip(pStrand);



  pStrand->pixels[currentLEDMode] = Green;

  digitalLeds_updatePixels(pStrand);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  BC24clearStrip(pStrand);


}




void BC24BottomFiveBlink(pixelColor_t myColor, int myDelay)
{
  strand_t * pStrand = &STRANDS[0];


#ifdef BC24DEBUG
  Serial.print("BottomFiveBlink(");

  Serial.print("ch");
  Serial.print(pStrand->rmtChannel);

  Serial.print(",");
  Serial.print(myColor.r);
  Serial.print(",");
  Serial.print(myColor.g);
  Serial.print(",");
  Serial.print(myColor.b);
  Serial.print(",");
  Serial.print(myColor.w);
  Serial.println(")");
#endif

  pStrand->pixels[16] = myColor;
  pStrand->pixels[17] = myColor;
  pStrand->pixels[18] = myColor;
  pStrand->pixels[19] = myColor;
  pStrand->pixels[20] = myColor;
  digitalLeds_updatePixels(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);
  BC24clearStrip(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);

}


void BC24TwoBlink(pixelColor_t myColor, int myDelay)
{
  strand_t * pStrand = &STRANDS[0];

  /* Serial.print("ThreeBlink(");

    Serial.print("ch");
    Serial.print(pStrand->rmtChannel);

    Serial.print(",");
    Serial.print(myColor.r);
    Serial.print(",");
    Serial.print(myColor.g);
    Serial.print(",");
    Serial.print(myColor.b);
    Serial.print(",");
    Serial.print(myColor.w);
    Serial.println(")");
  */

  pStrand->pixels[0] = myColor;
  pStrand->pixels[1] = myColor;

  digitalLeds_updatePixels(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);
  BC24clearStrip(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);

}


void BC24ThreeBlink(pixelColor_t myColor, int myDelay)
{
  strand_t * pStrand = &STRANDS[0];

  /* Serial.print("ThreeBlink(");

    Serial.print("ch");
    Serial.print(pStrand->rmtChannel);

    Serial.print(",");
    Serial.print(myColor.r);
    Serial.print(",");
    Serial.print(myColor.g);
    Serial.print(",");
    Serial.print(myColor.b);
    Serial.print(",");
    Serial.print(myColor.w);
    Serial.println(")");
  */

  pStrand->pixels[0] = myColor;
  pStrand->pixels[1] = myColor;
  pStrand->pixels[2] = myColor;

  digitalLeds_updatePixels(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);
  BC24clearStrip(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);

}
void BC24OneBlink(pixelColor_t myColor, int myDelay)
{
  strand_t * pStrand = &STRANDS[0];

  /* Serial.print("ThreeBlink(");

    Serial.print("ch");
    Serial.print(pStrand->rmtChannel);

    Serial.print(",");
    Serial.print(myColor.r);
    Serial.print(",");
    Serial.print(myColor.g);
    Serial.print(",");
    Serial.print(myColor.b);
    Serial.print(",");
    Serial.print(myColor.w);
    Serial.println(")");
  */

  pStrand->pixels[0] = myColor;

  digitalLeds_updatePixels(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);
  BC24clearStrip(pStrand);

  vTaskDelay((myDelay / 2) / portTICK_PERIOD_MS);

}

// display single pixel

void displaySinglePixel(int Pixel, pixelColor_t myStatusColor)
{

  strand_t * pStrand = &STRANDS[0];

  BC24clearStrip(pStrand);
  pStrand->pixels[Pixel] = myStatusColor;
  digitalLeds_updatePixels(pStrand);

}

// For Dow Jones

void displayDowJones(float currentDJ, float openDJ, pixelColor_t myStatusColor)
{

  strand_t * pStrand = &STRANDS[0];

  // split the difference into 23 zones
  BC24clearStrip(pStrand);
  // strip.Show();


  float currentFloatDelta;
  currentFloatDelta = (currentDJ - openDJ);


  int currentDelta;
  currentDelta = int(currentDJ - openDJ);
  int adjustedDelta;

#define MAXMINVALUE 500
  if (currentDelta > MAXMINVALUE)
    currentDelta = MAXMINVALUE;
  if (currentDelta < -MAXMINVALUE)
    currentDelta = -MAXMINVALUE;

  adjustedDelta = (currentDelta * 23 / MAXMINVALUE);


  pixelColor_t myColor;
  if (currentFloatDelta < 0)
  {
    myColor = Red;
    adjustedDelta = adjustedDelta - 1;

  }
  else
  {
    myColor = Green;
    adjustedDelta = adjustedDelta + 1;
  }

  // Now we set the other pixels for the color and direction

  for (int i = 0; i < abs(adjustedDelta); i++)
  {
    int PixelSet;
    if (adjustedDelta > 0)
    {
      int delta;
      delta = 0;
      if (5 - i < 0)
        delta = 24;
      PixelSet = 5 - i + delta;
      pStrand->pixels[PixelSet] = Green;

    }
    else
    {
      PixelSet =  abs((7 + i) % 24);
      pStrand->pixels[PixelSet] = Red;

    }


  }

  pStrand->pixels[6] = myStatusColor;
  digitalLeds_updatePixels(pStrand);
}



int regularizePixel(int r)
{
  if (r > 255)
    r = 255;
  if (r < 0)
    r = 0;

  return r;
}

pixelColor_t brightnessAbsolute(pixelColor_t pixelColor, int maximum)
{
  pixelColor_t newPixel;
  int r, g, b, w;
  int myMaximum;

  if ((r >= g) && (r >= b) && (r >= w))
  {
    myMaximum = r;
  }
  else if ((g >= b) && (g >= w))
  {
    myMaximum = g;
  }
  else if (b >= w)
  {
    myMaximum = b;
  }
  else
    myMaximum = w;



  newPixel.r = regularizePixel((int((float)r * ((float) myMaximum / (float)maximum))));
  newPixel.g = regularizePixel((int((float)g * ((float) myMaximum / (float)maximum))));
  newPixel.b = regularizePixel((int((float)b * ((float) myMaximum / (float)maximum))));
  newPixel.w = regularizePixel((int((float)w * ((float) myMaximum / (float)maximum))));


}



pixelColor_t brightnessPercentage(pixelColor_t pixelColor, int percentage)
{
  pixelColor_t newPixel;


  newPixel.r = regularizePixel(((pixelColor.r  * percentage) / 100));
  newPixel.g = regularizePixel(((pixelColor.g  * percentage) / 100));
  newPixel.b = regularizePixel(((pixelColor.b  * percentage) / 100));
  newPixel.w = regularizePixel(((pixelColor.w  * percentage) / 100));

  return newPixel;
}


// for circle LED
void displayCircleLED(int pixelNumber, pixelColor_t pixelColor)
{
  strand_t * pStrand = &STRANDS[0];

  pixelColor_t Prev1Color;
  pixelColor_t Prev2Color;

  Prev1Color = pixelColor;
  Prev2Color = pixelColor;


  /*
     Serial.print("Pre(");
     Serial.print(Prev1Color.r);
     Serial.print(",");
     Serial.print(Prev1Color.g);
     Serial.print(",");
     Serial.print(Prev1Color.b);
     Serial.print(",");
     Serial.print(Prev1Color.w);
     Serial.println(")");
  */

  Prev1Color = brightnessPercentage(Prev1Color, 3);
  Prev2Color = brightnessPercentage(Prev2Color, 15);

  /*
    Serial.print("Post(");
    Serial.print(Prev1Color.r);
    Serial.print(",");
    Serial.print(Prev1Color.g);
    Serial.print(",");
    Serial.print(Prev1Color.b);
    Serial.print(",");
    Serial.print(Prev1Color.w);
    Serial.println(")");

  */



  BC24clearStrip(pStrand);


  pStrand->pixels[((pixelNumber + 2) % 24)] = Prev1Color;
  pStrand->pixels[((pixelNumber + 1) % 24)] = Prev2Color;
  pStrand->pixels[((pixelNumber) % 24)] = pixelColor;
  pStrand->pixels[((pixelNumber - 1 ) % 24)] = Prev2Color;
  pStrand->pixels[((pixelNumber - 2) % 24)] = Prev1Color;



  digitalLeds_updatePixels(pStrand);



}

pixelColor_t addColors(pixelColor_t color1, pixelColor_t color2)
{

  pixelColor_t returnColor;

  returnColor = pixelFromRGBW(color1.r + color2.r, color1.g + color2.g, color1.b + color2.b, color1.w + color2.w);
#ifdef BC24DEBUG
  Serial.print("color 1- rgbw ");
  Serial.print(color1.r);
  Serial.print("/");
  Serial.print(color1.g);
  Serial.print("/");
  Serial.print(color1.b);
  Serial.print("/");
  Serial.println(color1.w);
  Serial.print("color 2- rgbw ");
  Serial.print(color2.r);
  Serial.print("/");
  Serial.print(color2.g);
  Serial.print("/");
  Serial.print(color2.b);
  Serial.print("/");
  Serial.println(color2.w);
  Serial.print("sum - rgbw ");
  Serial.print(returnColor.r);
  Serial.print("/");
  Serial.print(returnColor.g);
  Serial.print("/");
  Serial.print(returnColor.b);
  Serial.print("/");
  Serial.println(returnColor.w);

#endif

  return returnColor;

}

void   displayClock(int displayHour, int displayMinute, int displaySecond)
{
 
  strand_t * pStrand = &STRANDS[0];

  int secondPixel;
  int minutePixel;
  int hourPixel;

  pixelColor_t secondColor;
  pixelColor_t minuteColor;
  pixelColor_t hourColor;

  secondColor = DarkRed;
  minuteColor = DarkGreen;
  hourColor = DarkBlue;

  if (displayHour > 12)
    displayHour = displayHour - 12;
#ifdef BC24DEBUG
 Serial.println("Clock Second Tick");
  Serial.print("HT/MT/ST=");
  Serial.print(displayHour);
  Serial.print("/");
  Serial.print(displayMinute);
  Serial.print("/");
  Serial.println(displaySecond);
#endif
  secondPixel = int(24.0 * (1.0 - displaySecond / 60.0) + 7.5) % 24;
  minutePixel = ((int(24.0 * (1.0 - displayMinute / 60.0) + 6.5) - 1) % 24);
  hourPixel = ((int(24.0 * (1.0 - displayHour / 12.0) + 7.5) - 1) % 24);
#ifdef BC24DEBUG
  Serial.print("H/M/S=");
  Serial.print(hourPixel);
  Serial.print("/");
  Serial.print(minutePixel);
  Serial.print("/");
  Serial.println(secondPixel);
#endif

  if (secondPixel == 6)
    BC24clearStrip(pStrand);


  // now set up strips

  // for seconds, fill in from the top of the hour

  if (secondPixel <= 6)
  {
    for (int i = 0; i < 6 - secondPixel; i++)
    {
      int myPixel;
      myPixel = 6 - i;

      pStrand->pixels[myPixel] = secondColor;
    }
  }

  if (secondPixel > 6)
  {

    // do 0-6
    for (int i = 0; i < 6; i++)
    {
      int myPixel;
      myPixel = 6 - i;

      pStrand->pixels[myPixel] = secondColor;
    }

    for (int i = secondPixel; i < 24; i++)
    {

      pStrand->pixels[i] = secondColor;
    }
  }



  pStrand->pixels[hourPixel] = hourColor;

  if (hourPixel == minutePixel)
  {
    pStrand->pixels[minutePixel] = addColors(pStrand->pixels[hourPixel], minuteColor);
  }
  else
    pStrand->pixels[minutePixel] = minuteColor;

  if (secondPixel == hourPixel)
  {

    pStrand->pixels[hourPixel] = addColors(pStrand->pixels[hourPixel], secondColor);
  }
  else
    pStrand->pixels[hourPixel] = hourColor;

  if (secondPixel == minutePixel)
  {

    pStrand->pixels[minutePixel] = addColors(pStrand->pixels[minutePixel], secondColor);
  }
  else
    pStrand->pixels[secondPixel] = secondColor;

  digitalLeds_updatePixels(pStrand);


}

//void scanner(strand_t * pStrand, unsigned long delay_ms, unsigned long timeout_ms)
//void rainbows(strand_t * strands[], int numStrands, unsigned long delay_ms, unsigned long timeout_ms)//
void BC24CircleRainbow()
{
  strand_t * pStrand = &STRANDS[0];

  int latestQueueEntry;

  {

    rainbow(pStrand, 0, 2000);

    scanner(pStrand, 0, 2000);


    scanner(pStrand, 1, 2000);

    scanner(pStrand, 0, 2000);

    rainbow(pStrand, 0, 2000);


    scanner(pStrand, 1, 2000); // A tiny delay can smooth things out

    rainbow(pStrand, 0, 2000);

    scanner(pStrand, 5, 2000);

    rainbow(pStrand, 0, 2000);


    rainbow(pStrand, 0, 2000);


    rainbow(pStrand, 0, 2000);
    digitalLeds_resetPixels(pStrand);


  }

}

int convertLinearToPixel(int Pixel)
{
  int returnPixel;
  int tempValue;

  tempValue = (abs(Pixel - 24) + 6) % 24;

  return tempValue;

}

void setSinglePixel(int value, pixelColor_t pixelColor)
{

  strand_t * pStrand = &STRANDS[0];

  pStrand->pixels[value] = pixelColor;



}

void RightCircle(float value, float bottomValue, float topValue, pixelColor_t pixelColor, bool buildCircle, String Name)
{

  strand_t * pStrand = &STRANDS[0];
  int endPixel;

  /*
    Serial.print("Setting up:");
    Serial.println(Name);
    Serial.print("Value=");
    Serial.println(value);
  */

  float unitPixel;

  unitPixel = int(round((topValue - bottomValue) / 24.0));

  endPixel = int((value - bottomValue) / unitPixel);

  if (endPixel > 23)
    endPixel = 23;
  int i;
  for (i = 0; i < endPixel + 1; i++)
  {
    int doPixel = convertLinearToPixel(i);
    setSinglePixel(doPixel, pixelColor);
    if (buildCircle)
    {
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalLeds_updatePixels(pStrand);
    }
  }

  digitalLeds_updatePixels(pStrand);
}

void ThreeRightCircle(float value, float bottomValue, float topValue,  bool buildCircle, String Name, float range1, pixelColor_t pixelColor1, float range2, pixelColor_t pixelColor2, pixelColor_t pixelColor3)
{

  strand_t * pStrand = &STRANDS[0];
  int endPixel;

  /*
    Serial.print("Setting up:");
    Serial.println(Name);
    Serial.print("Value=");
    Serial.println(value);
  */

  float unitPixel;

  unitPixel = ((topValue - bottomValue) / 24.0);



  endPixel = int(round((value - bottomValue) / unitPixel));

  if (endPixel > 23)
    endPixel = 23;


  int i;
  for (i = 0; i < 24; i++)
  {
    int doPixel = convertLinearToPixel(i);

    if (i < endPixel)
    {
      // check ranges
      if (i * unitPixel < range1)
      {
        setSinglePixel(doPixel, pixelColor1);
      }
      else if (i * unitPixel < range2)
      {
        setSinglePixel(doPixel, pixelColor2);
      }
      else
      {
        setSinglePixel(doPixel, pixelColor3);
      }
    }
    else
    {
      setSinglePixel(doPixel, Black);
    }

    if (buildCircle)
    {
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalLeds_updatePixels(pStrand);
    }
  }

  digitalLeds_updatePixels(pStrand);
}



void ThreeRightCirclePlusDirection(float value, float bottomValue, float topValue,  bool buildCircle, float myDirection, String Name, float range1, pixelColor_t pixelColor1, float range2, pixelColor_t pixelColor2, pixelColor_t pixelColor3)
{

  strand_t * pStrand = &STRANDS[0];
  int endPixel;

  /*
    Serial.print("Setting up:");
    Serial.println(Name);
    Serial.print("Value=");
    Serial.println(value);
      Serial.print("myDirection=");
    Serial.println(myDirection);
  */
  float unitPixel;

  unitPixel = ((topValue - bottomValue) / 24.0);



  endPixel = int(round((value - bottomValue) / unitPixel));

  if (endPixel > 23)
    endPixel = 23;


  int i;
  for (i = 0; i < 24; i++)
  {
    int doPixel = convertLinearToPixel(i);

    if (i < endPixel)
    {
      // check ranges
      if (i * unitPixel < range1)
      {
        setSinglePixel(doPixel, pixelColor1);
      }
      else if (i * unitPixel < range2)
      {
        setSinglePixel(doPixel, pixelColor2);
      }
      else
      {
        setSinglePixel(doPixel, pixelColor3);
      }
    }
    else
    {
      setSinglePixel(doPixel, Black);
    }

    if (buildCircle)
    {
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalLeds_updatePixels(pStrand);
    }
  }

  // now set direction

  unitPixel = (360.0 / 24.0);
  endPixel = int(round(myDirection / unitPixel));

  setSinglePixel(convertLinearToPixel(endPixel), Blue);

  digitalLeds_updatePixels(pStrand);
}


void setUpDarkLight()
{
  // set up light or dark

  if (darkLight == 0)
  {
    Red = DarkRed;
    Blue = DarkBlue;
    Green = DarkGreen;
    White = DarkWhite;
  }
  else
  {

    Red = BrightRed;
    Blue = BrightBlue;
    Green = BrightGreen;
    White = BrightWhite;
  }

}


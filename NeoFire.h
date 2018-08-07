// Heavily modified from  https://www.arduinoslovakia.eu/blog/2015/9/simulacia-ohna?lang=en

class NeoFire
{

  public:

    NeoFire();
    void Draw();
    void Clear();
    void AddColor(uint8_t position, pixelColor_t color);
    void SubstractColor(uint8_t position, pixelColor_t color);
    pixelColor_t Blend(pixelColor_t color1, pixelColor_t color2);
    pixelColor_t Substract(pixelColor_t color1, pixelColor_t color2);
};

///
/// Constructor
///
NeoFire::NeoFire()

{
}

// led count
#define CNT 24

pixelColor_t fire_color   = pixelFromRGBW(80, 35, 0, 0);

pixelColor_t off_color    = pixelFromRGBW(0, 0, 0, 0);

///
/// Set all colors
///
void NeoFire::Draw()
{

  strand_t * pStrand = &STRANDS[0];
  Clear();

  for (int i = 0; i < CNT; i++)
  {
    AddColor(i, fire_color);
    int r = random(80);
    pixelColor_t diff_color = pixelFromRGBW( r, r / 2, r / 2, 0);
    SubstractColor(i, diff_color);
  }

  digitalLeds_updatePixels(pStrand);
}

///
/// Set color of LED
///
void NeoFire::AddColor(uint8_t position, pixelColor_t color)
{
  strand_t * pStrand = &STRANDS[0];

  pixelColor_t blended_color = Blend( pStrand->pixels[position], color);
  pStrand->pixels[position] = blended_color;
}



///
/// Set color of LED
///
void NeoFire::SubstractColor(uint8_t position, pixelColor_t color)
{
  strand_t * pStrand = &STRANDS[0];
  pixelColor_t blended_color = Substract(pStrand->pixels[position], color);
  pStrand->pixels[position] = blended_color;
}

///
/// Color blending
///
pixelColor_t NeoFire::Blend(pixelColor_t color1, pixelColor_t color2)
{

  return pixelFromRGBW(constrain(color1.r + color2.r, 0, 255), constrain(color1.g + color2.g, 0, 255), constrain(color1.b + color2.b, 0, 255), constrain(color1.w + color2.w, 0, 255));
}

///
/// Color blending
///

pixelColor_t NeoFire::Substract(pixelColor_t color1, pixelColor_t color2)
{
  return pixelFromRGBW(constrain(color1.r - color2.r, 0, 255), constrain(color1.g - color2.g, 0, 255), constrain(color1.b - color2.b, 0, 255), constrain(color1.w - color2.w, 0, 255));
}

///
/// Every LED to black
///
void NeoFire::Clear()
{

  strand_t * pStrand = &STRANDS[0];
  for (uint16_t i = 0; i < CNT; i++)
    pStrand->pixels[i] = off_color;
}

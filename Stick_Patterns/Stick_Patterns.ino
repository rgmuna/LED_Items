#define ARM_MATH_CM4
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <RunningAverage.h>

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, COLOR_WIPE_SINGLE,
                COLOR_WIPE_REVERSE, SCANNER,
                FADE, RANDOM_PIXELS, FLASH_COLOR, SCROLL_COLOR, SCROLL_TWO_COLOR,
                SCROLL_TWO_COLOR_REVERSE, SCROLL_MIDDLE, SCROLL_MIDDLE_SINGLE, SCROLL_MIDDLE_TWO_COLOR,
                SCROLL_ENDS, SCROLL_ENDS_TWO_COLOR, SCROLL_COLOR_UNDERCOLOR, COLOR_RAIN,
                BLOCK_DROP, COLOR_SWITCH, RANDOM_EATER, RANDOM_FILL, BLOCK_DROP_BORING, RANDOM_FILL_BORING,
                SPEED_CHANGE, BLOCK_PLACER, BLOCK_PLACER_BORING, SCANNER_BORING, BREATHE_COLOR, BREATHE_COLOR_RANDOM};
// Patern directions supported:
enum  direction { FORWARD, REVERSE };


const uint8_t PROGMEM Gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
    public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    int i;
    int Counter;
    int Counter2;

    void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }

    // Update the pattern
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            switch(ActivePattern)
            {
                case RAINBOW_CYCLE:
                    RainbowCycleUpdate();
                    break;
                case THEATER_CHASE:
                    TheaterChaseUpdate();
                    break;
                case COLOR_WIPE:
                    ColorWipeUpdate();
                    break;
                case COLOR_WIPE_SINGLE:
                    ColorWipeSingleUpdate();
                    break;
                case COLOR_WIPE_REVERSE:
                    ColorWipeReverseUpdate();
                    break;
                case SCANNER:
                    ScannerUpdate();
                    break;
               case SCANNER_BORING:
                    ScannerBoringUpdate();
                    break;
                case FADE:
                    FadeUpdate();
                    break;
                case RANDOM_PIXELS:
                    RandomPixelsUpdate();
                    break;
                case SCROLL_TWO_COLOR:
                    ScrollTwoColorUpdate();
                    break;
                case SCROLL_TWO_COLOR_REVERSE:
                    ScrollTwoColorReverseUpdate();
                    break;
                case SCROLL_MIDDLE:
                    ScrollMiddleUpdate();
                    break;
                case SCROLL_MIDDLE_SINGLE:
                    ScrollMiddleSingleUpdate();
                    break;
                case SCROLL_MIDDLE_TWO_COLOR:
                    ScrollMiddleTwoColorUpdate();
                    break;
                case SCROLL_ENDS:
                    ScrollEndsUpdate();
                    break;
                case SCROLL_ENDS_TWO_COLOR:
                    ScrollEndsTwoColorUpdate();
                    break;
                case SCROLL_COLOR_UNDERCOLOR:
                    ScrollColorUndercolorUpdate();
                    break;
                case COLOR_RAIN:
                    ColorRainUpdate();
                    break;
                case BLOCK_DROP:
                    BlockDropUpdate();
                    break;
                case COLOR_SWITCH:
                    ColorSwitchUpdate();
                    break;
                case RANDOM_EATER:
                    RandomEaterUpdate();
                    break;
                case BLOCK_DROP_BORING:
                    BlockDropBoringUpdate();
                    break;
                case RANDOM_FILL:
                     RandomFillUpdate();
                     break;
                case RANDOM_FILL_BORING:
                     RandomFillBoringUpdate();
                     break;
                case SPEED_CHANGE:
                     SpeedChangeUpdate();
                     break;
                case BLOCK_PLACER:
                     BlockPlacerUpdate();
                     break;
                case BLOCK_PLACER_BORING:
                     BlockPlacerBoringUpdate();
                     break;
                 case BREATHE_COLOR:
                      BreatheColorUpdate();
                      break;
                 case BREATHE_COLOR_RANDOM:
                      BreatheColorUpdate();
                default:
                    break;
            }
        }
    }

    // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the completion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }

        // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD) {
        ActivePattern = RAINBOW_CYCLE;
        Interval      = interval;
        TotalSteps    = 255;
        Index         = 0;
    }

        // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate() {
        for (int i=0; i< numPixels(); i++) {
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        }
        show();
        Increment();
    }

    // Initialize for a ColorWipe
    void ColorWipeSingle(uint32_t color, float interval) {
        ActivePattern = COLOR_WIPE_SINGLE;
        Interval      = interval;
        TotalSteps    = numPixels();
        Color1        = color;
        Index         = 0;
    }

    // Update the Color Wipe Pattern
    void ColorWipeSingleUpdate() {
        if (Index == 0) {
          CleanPixels();
        }
        setPixelColor(Index, Color1);
        show();
        Increment();
    }

    //-------------------- COLOR WIPE (goes down one color, returns a different color
    void ColorWipe(uint32_t color1, uint32_t color2, float interval) {
        ActivePattern = COLOR_WIPE;
        Interval      = interval;
        TotalSteps    = numPixels()*2;
        Color1        = color1;
        Color2        = color2;
        Index         = 0;
    }

    void ColorWipeUpdate() {
        if(Index < numPixels()){
          setPixelColor(Index, Color1);
        }
        else{
          setPixelColor(TotalSteps-Index, Color2);
        }
        show();
        Increment();
    }

    // Initialize reverse color wipe
    void ColorWipeReverse(int color1, float interval) {
      ActivePattern = COLOR_WIPE_REVERSE;
      Interval      = interval;
      TotalSteps    = numPixels() + 1;
      Color1        = color1;
      Index         = 0;
    }

    void ColorWipeReverseUpdate() {
      if (Index == 0) {
        CleanPixels();
      }

      setPixelColor(numPixels() - Index, Color1);
      show();
      Increment();
    }

    // Initialize for a color scroll with a color underneath
    void ScrollColorUndercolor(int color1, int color2, float interval) {
      ActivePattern = SCROLL_COLOR_UNDERCOLOR;
      Interval      = interval;
      TotalSteps    = numPixels();
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the color scroll with a different undercolor
    void ScrollColorUndercolorUpdate() {
      if (Index == 0) {
        ColorSet(Color2);
      }

      setPixelColor(Index, Color1);
      show();
      Increment();
    }

    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, float interval) {
        ActivePattern = THEATER_CHASE;
        Interval      = interval;
        TotalSteps    = numPixels();
        Color1        = color1;
        Color2        = color2;
        Index         = 0;
    }

   // Update the Theater Chase Pattern
    void TheaterChaseUpdate() {
      for(int i=0; i< numPixels(); i++) {
        if ((i + Index) % 3 == 0) {
            setPixelColor(i, Color1);
        } else {
            setPixelColor(i, Color2);
        }
      }

      show();
      Increment();
    }

    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, float interval) {
        ActivePattern = SCANNER;
        Interval      = interval;
        TotalSteps    = (numPixels() - 1) * 2;
        Color1        = color1;
        Index         = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate() {
      for (int i = 0; i < numPixels(); i++) {
        if (i == Index) { // first half of the scan
            // Serial.print(i);
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        } else if (i == TotalSteps - Index) { // The return trip.
            // Serial.print(i);
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        } else  {// fade to black
            setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }

      show();
      Increment();
    }

    void ScannerBoring(uint32_t color1, float interval) {
        ActivePattern = SCANNER_BORING;
        Interval      = interval;
        TotalSteps    = (numPixels() - 1) * 2;
        Color1        = color1;
        Index         = 0;
    }

        // Update the Scanner Pattern
    void ScannerBoringUpdate() {
      for (int i = 0; i < numPixels(); i++) {
        if (i == Index) {// first half of the scan
          // Serial.print(i);
          setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - Index) {// The return trip.
          // Serial.print(i);
          setPixelColor(i, Color1);
        }
        else {  // fade to black
          setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }

      show();
      Increment();
    }

    // Initialize for a Fade
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, int interval) {
      ActivePattern = FADE;
      Interval      = interval;
      TotalSteps    = steps;
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the Fade Pattern
    void FadeUpdate() {
      uint8_t red   = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
      uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
      uint8_t blue  = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

      ColorSet(Color(red, green, blue));
      show();
      Increment();
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Custom Patterns Begin
/////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Random Pixels
    void RandomPixels(int interval, int steps) {
      ActivePattern = RANDOM_PIXELS;
      Interval      = interval;
      TotalSteps    = steps;
    }

    // Update the Random Pixels pattern
    void RandomPixelsUpdate() {
      setPixelColor(random(numPixels()-1), Wheel(random(255)));
      setPixelColor(random(numPixels()-1), 0);
      show();
      Increment();
    }

    // Two-color scroll (or ColorWipe)
    void ScrollTwoColor(uint32_t color1, uint32_t color2, float interval) {
      ActivePattern = SCROLL_TWO_COLOR;
      Interval      = interval;
      TotalSteps    = numPixels() + 3;
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the Scroll Two Color Pattern
    void ScrollTwoColorUpdate() {
      if (Index == 0) {
        CleanPixels();
        setPixelColor(Index, Color1);
      }

      else if (Index == 1) {
        setPixelColor(Index, Color1);
        setPixelColor(Index-1, Color1);
      }
      else if (Index == 2) {
        setPixelColor(Index, Color1);
        setPixelColor(Index-1, Color1);
        setPixelColor(Index-2, Color1);
      }
      else if (Index == numPixels() - 1) {        //the last pixel
        setPixelColor(Index, Color1);
        setPixelColor(Index-1, Color1);
        setPixelColor(Index-2, Color1);
        setPixelColor(Index-3, Color2);
      }
      else if (Index == numPixels()) {
        setPixelColor(Index-1, Color1);
        setPixelColor(Index-2, Color1);
        setPixelColor(Index-3, Color2);
      }
      else if (Index == numPixels() + 1) {
        setPixelColor(Index-2, Color1);
        setPixelColor(Index-3, Color2);
      }
      else if (Index == numPixels() + 2) {
        setPixelColor(Index-3, Color2);
      }
      else {
        setPixelColor(Index, Color1);
        setPixelColor(Index-1, Color1);
        setPixelColor(Index-2, Color1);
        setPixelColor(Index-3, Color2);
      }
      show();
      Increment();
    }

    // Two color scroll in reverse
    void ScrollTwoColorReverse(uint32_t color1, uint32_t color2, float interval) {
      ActivePattern = SCROLL_TWO_COLOR_REVERSE;
      Interval      = interval;
      TotalSteps    = numPixels() + 4;
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the reverse two-color scroll
    void ScrollTwoColorReverseUpdate() {
      if (Index == 0) {
        CleanPixels();
      }

      setPixelColor(numPixels()-Index, Color1);
      setPixelColor(numPixels()-Index+1, Color1);
      setPixelColor(numPixels()-Index+2, Color1);
      setPixelColor(numPixels()-Index+3, Color2);

      show();
      Increment();
    }

    //-------------------- SCROLL MIDDLE ---------------------------------------------------------
    void ScrollMiddle(uint32_t color1, uint32_t color2, float interval) {
      ActivePattern = SCROLL_MIDDLE;
      Interval      = interval;
      TotalSteps    = numPixels();
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the scroll middle pattern
    void ScrollMiddleUpdate() {
      if (Index == 0) {
        CleanPixels();
      }
      else if(Index < numPixels()/2){
        setPixelColor((numPixels()/2)+Index, Color1);
        setPixelColor((numPixels()/2)-Index, Color1);
      }
      else{
        setPixelColor(Index-(numPixels()/2), Color2);
        setPixelColor(TotalSteps-(Index-(numPixels()/2)), Color2);
      }

      show();
      Increment();
    }

    // Scroll out from middle to ends
    void ScrollMiddleSingle(uint32_t color1, float interval) {
      ActivePattern = SCROLL_MIDDLE_SINGLE;
      Interval      = interval;
      TotalSteps    = (numPixels()/2) + 1;
      Color1        = color1;
      Index         = 0;
    }

    // Update the scroll middle pattern
    void ScrollMiddleSingleUpdate() {
      if (Index == 0) {
        CleanPixels();
      }

      setPixelColor((numPixels()/2)+Index, Color1);
      setPixelColor((numPixels()/2)-Index, Color1);
      show();
      Increment();
    }

    // Two-color scroll out from middle to ends
    void ScrollMiddleTwoColor(uint32_t color1, uint32_t color2, float interval) {
      ActivePattern = SCROLL_MIDDLE_TWO_COLOR;
      Interval      = interval;
      TotalSteps    = (numPixels()/2) + 1;
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the two-color middle scroll pattern
    void ScrollMiddleTwoColorUpdate() {
      if (Index == 0) {
        CleanPixels();
        setPixelColor(numPixels()/2, Color1);
        setPixelColor((numPixels()/2)-1, Color1);
      }

      else {
        setPixelColor((numPixels()/2)+Index, Color1);
        setPixelColor((numPixels()/2)-Index, Color1);
        setPixelColor((numPixels()/2)+Index-1, Color2);
        setPixelColor((numPixels()/2)-Index+1, Color2);
      }
      show();
      Increment();
    }


    // Scroll in from ends
    void ScrollEnds(uint32_t color1, float interval) {
      ActivePattern = SCROLL_ENDS;
      Interval      = interval;
      TotalSteps    = (numPixels()/2) + 1;
      Color1        = color1;
      Index         = 0;
    }

    // Update the scroll ends pattern
    void ScrollEndsUpdate() {
      if (Index == 0) {
        CleanPixels();
      }

      setPixelColor(Index, Color1);
      setPixelColor(numPixels()-Index, Color1);
      show();
      Increment();
    }

    // Two-color scroll in from the ends
    void ScrollEndsTwoColor(int color1, int color2, float interval) {
      ActivePattern = SCROLL_ENDS_TWO_COLOR;
      Interval      = interval;
      TotalSteps    = (numPixels()/2) + 1;
      Color1        = color1;
      Color2        = color2;
      Index         = 0;
    }

    // Update the two-color scroll ends pattern
    void ScrollEndsTwoColorUpdate() {
      if (Index == 0) {
        CleanPixels();
        setPixelColor(Index, Color1);
        setPixelColor(numPixels(), Color1);
      }

      else {
        setPixelColor(Index, Color1);
        setPixelColor(Index-1, Color2);
        setPixelColor(numPixels() - 1 - Index, Color2);
        setPixelColor(numPixels() - 1 - Index - 1, Color1);
      }
      show();
      Increment();
    }

    //-------------------- Random Pixels ------------------------------------------------------
  void RandomEater(uint32_t color1, uint32_t color2, float interval){
    ActivePattern = RANDOM_EATER;
    Interval      = interval;
    TotalSteps    = numPixels();
    Color1        = color1;
    Color2        = color2;
    Index         = 0;
  }

  void RandomEaterUpdate(){
    if(Index==0){
      CleanPixels();
    }
    else{
      setPixelColor(random(TotalSteps-Index), Color2);
      setPixelColor(TotalSteps-Index, Color1);
      setPixelColor(TotalSteps-Index+1, DimControl(Color1, 2));
      setPixelColor(TotalSteps-Index+2, DimControl(Color1, 4));
      setPixelColor(TotalSteps-Index+3, DimControl(Color1, 8));
      setPixelColor(TotalSteps-Index+4, DimControl(Color1, 15));
      setPixelColor(TotalSteps-Index+5, DimControl(Color1, 20));
      setPixelColor(TotalSteps-Index+6, DimControl(Color1, 30));
      setPixelColor(TotalSteps-Index+7, 0);
    }
    show();
    Increment();
  }

  //-------------------- COLOR RAIN---------------------------------------------------------

  void ColorRain(uint32_t color1, uint32_t color2,  uint8_t interval) {
    ActivePattern = COLOR_RAIN;
    Interval = interval;
    TotalSteps = numPixels()+5;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
  }

  void ColorRainUpdate(){
    int rainSize = 5;

    for(i=0; i<=rainSize; i++){
      setPixelColor(Index+i-30, Color1);
      setPixelColor(Index+i-rainSize-30, DimControl(Color2, 10));
      setPixelColor(Index+i-rainSize, Color1);
      setPixelColor(Index+i-rainSize-rainSize, DimControl(Color2, 10));
      setPixelColor(Index+i+27, Color1); //change to 27 for roque
      setPixelColor(Index+i-rainSize+27, DimControl(Color2, 10)); //change to 27 for roque
    }

    show();
    Increment();
  }




  //-------------------- Block Drop --------------------------------------------------
  void BlockDrop(uint32_t color1, uint8_t interval) {
    ActivePattern = BLOCK_DROP;
    Interval      = interval;
    TotalSteps    = (numPixels()*(numPixels()+1))/2;
    //TotalSteps  = 1431;
    Color1        = color1;
    Index         = 0;
    i             = 0; //keeps track of number which LEDs to light up
    Counter       = 0; //keeps track of which particular LED is going to the bottom
    Counter2      = 0;
  }

  void BlockDropUpdate() {
     if(Index==0){
       //CleanPixels();
       Counter=0;
       Counter2=0;
     }
    i = Index - Counter;
    setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
    setPixelColor(i-1, 0);

    if(i>numPixels()-2-Counter2){
      Counter = Index;
      Counter2++;
    }
    show();
    Increment();
  }

  void BlockDropBoring(uint32_t color1, uint8_t interval) {
      ActivePattern = BLOCK_DROP_BORING;
      Interval      = interval;
      TotalSteps    = (numPixels()*(numPixels()+1))/2;
      //TotalSteps  = 1431;
      Color1        = color1;
      Index         = 0;
      i             = 0; //keeps track of number which LEDs to light up
      Counter       = 0; //keeps track of which particular LED is going to the bottom
      Counter2      = 0;
  }

  void BlockDropBoringUpdate(){
    if(Index==0){
     //CleanPixels();
      Counter = 0;
      Counter2 = 0;
    }

    i = Index - Counter;
    setPixelColor(i, Color1);
    setPixelColor(i-1, 0);

    if(i>numPixels()-2-Counter2){
      Counter = Index;
      Counter2++;
    }

    show();
    Increment();
  }

  //------------ Breathe Color ----------------------//
    void BreatheColor(int color1, float interval) {
      ActivePattern = BREATHE_COLOR;
      Interval      = interval;
      TotalSteps    = 512;
      Index         = 0;
      Color1        = color1;
    }

    // Update the Breathe Color pattern
    void BreatheColorUpdate() {
      if(Index <= 255) {
        for(int i = 0; i < numPixels(); i++) {
          setPixelColor(i, DimControl(Color1, Index));
        }
      }

      else {
        for(int i=0; i < numPixels(); i++) {
          setPixelColor(i, DimControl(Color1, 512-Index));
        }
      }

      show();
      Increment();
    }

    //------------ Breathe Color ----------------------//
    void BreatheColorRandom(int color1, int color2, float interval) {
      ActivePattern = BREATHE_COLOR_RANDOM;
      Interval      = interval;
      TotalSteps    = 512;
      Index         = 0;
      Color1        = color1;
      Color2        = color2;
    }

    // Update the Breathe Color pattern
    void BreatheColorRandomUpdate() {
      uint8_t red   = ((Red(Color1)   * (TotalSteps - Index)) + (Red(Color2)   * Index)) / TotalSteps;
      uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
      uint8_t blue  = ((Blue(Color1)  * (TotalSteps - Index)) + (Blue(Color2)  * Index)) / TotalSteps;

      if(Index <= 255) {
        for(int i = 0; i < numPixels(); i++) {
          setPixelColor(i, DimControl(Color(red, green, blue), Index));
        }
      }

      else {
        for(int i = 0; i < numPixels(); i++) {
          setPixelColor(i, DimControl(Color(red, green, blue), 512-Index));
        }
      }

      show();
      Increment();
    }

  //-------------- COLOR SWITCH --------------------------

  void ColorSwitch(uint32_t color1, uint32_t color2, uint8_t interval) {
    ActivePattern = COLOR_SWITCH;
    Interval      = interval;
    TotalSteps    = numPixels();
    Color1        = color1;
    Color2        = color2;
    Index         = 0;
  }

  void ColorSwitchUpdate(){
    if(Index < TotalSteps/2){
      setPixelColor(Index, Color1);
      setPixelColor(TotalSteps-Index, Color1);
    } else {
      setPixelColor(Index, Color2);
      setPixelColor(TotalSteps-Index, Color2);
    }

    show();
    Increment();
  }

//-------------------- Random Fill -----------------------------------------
    void RandomFill(uint32_t color1, int interval) {
      TotalSteps    = numPixels();
      Index         = 0;
      ActivePattern = RANDOM_FILL;
      Interval      = interval;
      Color1        = color1;
      i             = 0; // A random pixel to update
    }

    // Update the Random Pixels pattern
    void RandomFillUpdate() {
      if (Index == 0) {
        CleanPixels();
      } else {
        while (true) {
          i = random(numPixels()-1);

          if (getPixelColor(i) == 0) {
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
            break;
          }
        }
      }

      show();
      Increment();
    }

//-------------------- Random Fill Boring -----------------------------------------
    void RandomFillBoring(uint32_t color1, int interval) {
      TotalSteps    = numPixels();
      Index         = 0;
      ActivePattern = RANDOM_FILL_BORING;
      Interval      = interval;
      Color1        = color1;
      i             = 0; // A random pixel to update
    }

    // Update the Random Pixels pattern
    void RandomFillBoringUpdate() {
      if (Index == 0) {
        CleanPixels();
      } else {
        while (true) {
          i = random(numPixels()-1);

          if (getPixelColor(i) == 0) {
            setPixelColor(i, Color1);
            break;
          }
        }
      }

      show();
      Increment();
    }


//-------------------- BlockPlacer Boring-----------------------------------------
  void BlockPlacerBoring(uint32_t color1, uint32_t color2, int interval) {
    TotalSteps    = numPixels()+1;
    Index         = 0;
    ActivePattern = BLOCK_PLACER_BORING;
    Interval      = interval;
    Color1        = color1;
    Color2        = color2;
    Counter2      = numPixels(); //keeps track of how many pixels on bottom
    Counter       = 0; //keeps track of how many pixels on top
    i             = 0; //keeps track of whether blocks are going up or down
  }


  void BlockPlacerBoringUpdate() {
    if (Counter2 == numPixels()/2) {
      CleanPixels();
      Counter2 = numPixels(); //keeps track of how many pixels on bottom
      Counter  = 0; //keeps track of how many pixels on top
      i        = 0; //keeps track of whether blocks are going up or down
      Index    = TotalSteps-1;
    } else {
      if (i==0) { //blocks going down
        if (Index+Counter != Counter2) {
          setPixelColor(Counter+Index, Color1);
          setPixelColor(Counter+Index-1, 0);

          for (int j=0; j<Counter; j++) {
            setPixelColor(j, Color1);
          }
        } else {
          i     = 1;
          Counter2--;
          Index = 0;
        }
      }

      if (i==1) { //blocks going up
        if (Counter2-Index != Counter) {
          setPixelColor(Counter2-Index, Color1);
          setPixelColor(Counter2-Index+1, 0);

          for (int j=Counter2; j<numPixels(); j++) {
            setPixelColor(j, Color1);
          }
        } else {
          i     = 0;
          Counter++;
          Index = 0;
        }
      }
    }

    show();
    Increment();
  }


//-------------------- BlockPlacer -----------------------------------------

  void BlockPlacer(uint32_t color1, uint32_t color2, int interval) {
    TotalSteps    = numPixels()+1;
    Index         = 0;
    ActivePattern = BLOCK_PLACER;
    Interval      = interval;
    Color1        = color1;
    Color2        = color2;
    Counter2      = numPixels(); //keeps track of how many pixels on bottom
    Counter       = 0; //keeps track of how many pixels on top
    i             = 0; //keeps track of whether blocks are going up or down
  }


  void BlockPlacerUpdate() {
    if (Counter2 == numPixels()/2) {
      CleanPixels();
      Counter2 = numPixels(); //keeps track of how many pixels on bottom
      Counter  = 0; //keeps track of how many pixels on top
      i        = 0; //keeps track of whether blocks are going up or down
      Index    = TotalSteps-1;
    } else {
      if (i==0) { //blocks going down
        if (Index+Counter != Counter2) {
          setPixelColor(Counter+Index, Wheel(((i * 256 / numPixels()) + Index) & 255));
          setPixelColor(Counter+Index-1, 0);

          for(int j=0; j<Counter; j++){
            setPixelColor(j, Wheel(((i * 256 / numPixels()) + Index) & 255));
          }
        } else {
          i     = 1;
          Counter2--;
          Index = 0;
        }
      }

      if (i==1){  //blocks going up
        if (Counter2-Index != Counter) {
          setPixelColor(Counter2-Index, Wheel(((i * 256 / numPixels()) + Index) & 255));
          setPixelColor(Counter2-Index+1, 0);

          for (int j=Counter2; j<numPixels(); j++) {
              setPixelColor(j, Wheel(((i * 256 / numPixels()) + Index) & 255));
          }
        }
        else {
          i     = 0;
          Counter++;
          Index = 0;
        }
      }
    }

    show();
    Increment();
  }

//-------------------- SpeedChange -----------------------------------------
  void SpeedChange(uint32_t color1, uint32_t color2, int interval) {
    TotalSteps    = numPixels()+1;
    Index         = 0;
    ActivePattern = SPEED_CHANGE;
    Interval      = interval;
    Color1        = color1;
    Color2        = color2;
    Counter       = numPixels();
    Counter2      = 0;
  }

   void SpeedChangeUpdate() {
    if (Index == Counter) {
      for(int j = 0; j < Counter2; j++) {
        setPixelColor(j, Color1);
      }

      Counter2++;
      Index = 0;
      Counter--;
    }

    show();
    Increment();
  }

//-------------------- ------------------------
        // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color) {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color) {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color) {
      return color & 0xFF;
    }

    // Return color, dimmed by 75% (used by scanner)
    uint32_t DimColor(uint32_t color) {
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    uint32_t DimColorMore(uint32_t color) {
      uint32_t dimColor = Color(Red(color)/4, Green(color)/4, Blue(color)/4);
      return dimColor;
    }

    uint32_t DimColorMost(uint32_t color) {
      uint32_t dimColor = Color(Red(color)/9, Green(color)/9, Blue(color)/9);
      return dimColor;
    }

//    uint32_t DimControl(uint32_t color, int i)                                       //MIGHT NEED THIS?------------------------------------------
//    {
//      uint32_t dimColor = Color(Red(color)/i, Green(color)/i, Blue(color)/i);
//        return dimColor;
//    }


     uint32_t DimControl(uint32_t color, int dimBright) {
      uint32_t dimColor;
//      uint32_t redColor = Red(color);
//      uint32_t greenColor = Green(color);
//      uint32_t blueColor = Blue(color);
//      if(i==0) {
//        dimColor = Color(0, 0, 0);
//      }
//      else {
//        redColor = redColor - i; //(255-i);
//        greenColor = greenColor - i; //(255-i);
//        blueColor = blueColor - i; //(255-i);
//        if (redColor < 0) redColor = 0;
//        if (greenColor < 0) greenColor = 0;
//        if (blueColor < 0) blueColor = 0;
        // Serial.print("dimBright: ");
        // Serial.println(dimBright);

        //dimColor = Color(Red(color)/i, Green(color)/i, Blue(color)/i);
//        dimColor = Color(pgm_read_byte(&Gamma[dimBright*(Red(color))/255]), pgm_read_byte(&Gamma[dimBright*(Green(color))/255]), pgm_read_byte(&Gamma[dimBright*(Blue(color))/255]));
        dimColor = Color(dimBright*(Red(color))/255, dimBright*(Green(color))/255, dimBright*(Blue(color))/255);

//      }
      return dimColor;
    }
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos) {
      WheelPos = 255 - WheelPos;
      if(WheelPos < 85) {
        return Color(pgm_read_byte(&Gamma[255 - WheelPos * 3]), 0, pgm_read_byte(&Gamma[WheelPos * 3]));
      } else if(WheelPos < 170) {
        WheelPos -= 85;
        return Color(0, pgm_read_byte(&Gamma[WheelPos * 3]), pgm_read_byte(&Gamma[255 - WheelPos * 3]));
      } else {
        WheelPos -= 170;
        return Color(pgm_read_byte(&Gamma[WheelPos * 3]), pgm_read_byte(&Gamma[255 - WheelPos * 3]), 0);
      }
    }

        // Reverse direction of the pattern
    void Reverse() {
      if (Direction == FORWARD) {
        Direction = REVERSE;
        Index     = TotalSteps-1;
      } else {
        Direction = FORWARD;
        Index     = 0;
      }
    }

        // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color) {
      for (int i = 0; i < numPixels(); i++) {
        setPixelColor(i, color);
      }

      show();
    }

    void CleanPixels() {
      for (int i = 0; i < numPixels(); i++) {
        setPixelColor(i, 0);
      }

      show();
    }
};

///////////////////// end of NeoPattern class ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int buttonPattern                         = 7; //button #1 for pattern cycling
int buttonBrightness                      = 8; //button #2 for brightness
int buttonBank                            = 9; //button #3 for changing pattern bank

volatile int patternNumber                = 1; // keeps track of pattern
volatile int bankNumber                   = 1; //keeps track of both buttons being pressed
//volatile int brightnessNumber           = 0; //keeps track of brightness level
volatile int brightnessNumber             = 255;
volatile int colorLock                    = 0;

//variables to keep track of the timing of recent interrupts
unsigned long button_time_pattern         = 0;
unsigned long last_button_time_pattern    = 0;
unsigned long button_time_brightness      = 0;
unsigned long last_button_time_brightness = 0;
unsigned long button_time_bank            = 0;
unsigned long last_button_time_bank       = 0;

int debounceTime                          = 250; //in milliseconds

const int POWER_LED_PIN                   = 13; // Output pin for power LED (pin 13 to use Teensy 3.0's onboard LED).
const int NEO_PIXEL_PIN                   = 3; // Output pin for neo pixels.
const int NEO_PIXEL_COUNT                 = 53; // Number of neo pixels. You should be able to increase this without
const int MAX_CHARS                       = 65; // Max size of the input command buffer

void pixelsComplete();

NeoPatterns pixels(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800, &pixelsComplete);

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int wait = 80;

////////////// Change these accordingly //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int numberBanks = 3; //set this to the number of banks
int numberBrightnessLevels = 5; //set this to number of bightness levels

int numberPatterns1 = 9;
int numberPatterns2 = 13;
int numberPatterns3 = 15;

int brightnessLevels[] = {255, 127, 64, 5, 2, 0}; //Set to maximum brightness
bool randomPattern = false;

int numPatterns[] = {numberPatterns1, numberPatterns2, numberPatterns3 };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Serial.begin(38400);  //turn on serial communication

  //pin setups
  pinMode(7, INPUT_PULLUP);     //define pin 7 as input
  pinMode(8, INPUT_PULLUP);     //define pin 8 as input
  pinMode(9, INPUT_PULLUP);     //define pin 9 as input

  // Turn on the power indicator LED.
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);

  pixels.begin(); //start NeoPattern class
  pixels.setBrightness(brightnessNumber);
  pixels.show();

  pixels.RandomPixels(30, 500);
}

void loop() {
  pixels.Update();

  if (digitalRead(7) == LOW) {
    randomPattern = false;
    patternControl();
    bankChoice();
  }

  if (digitalRead(8) == LOW) {
    brightnessControl();
    pixels.setBrightness(brightnessLevels[brightnessNumber]);
  }

  if (digitalRead(9) == LOW) {
    bankControl();
    patternControl();
    bankChoice();
  }
}

void pixelsComplete() {
  if (colorLock == 0) {
    pixels.Color1 = pixels.Wheel(random(255));
    pixels.Color2 = pixels.Wheel(random(255));
    pixels.Update();
  }

  if (randomPattern) {
    patternNumber = random(1, numPatterns[bankNumber-1]);
    // Serial.println(patternNumber);
    bankChoice();
  }

  else {
    pixels.Update();
  }
}



////////////////////////////////////////////////////////////////////////////////
//// PATTERN CALLS
///////////////////////////////////////////////////////////////////////////////

void patternChoiceBank1() {
  int c1 = random(255);
  int c2 = c1/2;

  pixels.Index = 0;
  pixels.CleanPixels();

  switch(patternNumber) {
    case 1:
            pixels.RainbowCycle(40);
            break;
    case 2:
            pixels.ColorRain(pixels.Wheel(255), pixels.Wheel(255), 40);
            break;
    case 3:
            pixels.ColorRain(pixels.Wheel(70), pixels.Wheel(70), 40);
            break;
    case 4:
            pixels.ColorRain(pixels.Wheel(170), pixels.Wheel(170), 40);
            break;
    case 5:
            pixels.ColorRain(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 40);
            break;
    case 6:
            pixels.BreatheColor(pixels.Wheel(random(255)), 5);
            break;
    case 7:
            pixels.BreatheColorRandom(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 5);
            break;
    case 8:
            pixels.Fade(pixels.Wheel(c1), pixels.Wheel(c2), 100, 100);
            break;
    case 9:
            randomPattern = true;
            pixels.Update();
            break;
  }
}
//
void patternChoiceBank2() {
  pixels.CleanPixels();
  pixels.Index = 0;

  switch(patternNumber) {
    case 1:
            pixels.TheaterChase(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 150);
            break;
    case 2:
            pixels.BlockPlacerBoring(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 30);
            break;
    case 3:
            pixels.BlockPlacer(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 30);
            break;
    case 4:
            pixels.BlockDropBoring(pixels.Wheel(random(255)), 70);
            break;
    case 5:
            pixels.BlockDrop(pixels.Wheel(random(255)), 70);
            break;
    case 6:
            pixels.ScannerBoring(pixels.Wheel(random(255)), 40);
            break;
    case 7:
            pixels.Scanner(pixels.Wheel(random(255)), 40);
            break;
    case 8:
            pixels.ColorWipe(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 40);
            break;
    case 9:
            pixels.ColorSwitch(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 40);
            break;
    case 10:
            pixels.RandomFill(pixels.Wheel(random(255)), 70);
            break;
    case 11:
            pixels.ScrollMiddleTwoColor(pixels.Wheel(random(255)), pixels.Wheel(random(255)), random(50, 70));
            break;
    case 12:
            pixels.ScrollEndsTwoColor(pixels.Wheel(random(255)), pixels.Wheel(random(255)), random(50, 70));
            break;
    case 13:
            randomPattern = true;
            pixels.Update();
            break;
  }
}

void patternChoiceBank3() {       // beat reactive patterns
  pixels.CleanPixels();
  pixels.Index = 0;

  switch(patternNumber) {
    case 1:
            pixels.Scanner(pixels.Wheel(random(255)), 20);
            break;
    case 2:
            pixels.TheaterChase(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 70);
            break;
    case 3:
            pixels.BlockPlacerBoring(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 5);
            break;
    case 4:
            pixels.BlockPlacer(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 7);
            break;
    case 5:
            pixels.BlockDropBoring(pixels.Wheel(random(255)), 15);
            break;
    case 6:
            pixels.BlockDrop(pixels.Wheel(random(255)), 15);
            break;
    case 7:
            pixels.ColorWipe(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 20);
            break;
    case 8:
            pixels.ColorSwitch(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 20);
            break;
    case 9:
            pixels.RandomFill(pixels.Wheel(random(255)), 30);
            break;
    case 10:
            pixels.ScrollMiddleTwoColor(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 30);
            break;
    case 11:
            pixels.ScrollEndsTwoColor(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 30);
            break;
    case 12:
            randomPattern = true;
            pixels.Update();
            break;
  }
}

void bankChoice() {
  switch(bankNumber) {
    case 1: colorLock = 1;
            patternChoiceBank1();
            break;
    case 2: colorLock = 0;
            patternChoiceBank2();
            break;
    case 3: colorLock = 0;
            patternChoiceBank3();
            break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//// INTERRUPT SERVICE ROUTINES
///////////////////////////////////////////////////////////////////////////////

////Pattern button interrupt
void patternControl() {
  button_time_pattern = millis();

  if (button_time_pattern - last_button_time_pattern > debounceTime) //check to see if ISR was called in the last 250 milliseconds
  {
    switch(bankNumber) { //add cases for each bank added
      case 1:
              if (patternNumber < numberPatterns1) {
                patternNumber++;
              }
              else {
                patternNumber = 1;
              }
              break;
      case 2:
              if (patternNumber < numberPatterns2) {
                patternNumber++;
              }
              else {
                patternNumber = 1;
              }
              break;
      case 3:
              if (patternNumber < numberPatterns3) {
                patternNumber++;
              }
              else {
                patternNumber = 1;
              }
              break;
    }
   last_button_time_pattern = button_time_pattern;
  }
}

//brightness button interrupt
void brightnessControl() {
  button_time_brightness = millis();

  if (button_time_brightness - last_button_time_brightness > debounceTime)
  {
    if (brightnessNumber < numberBrightnessLevels) {
      brightnessNumber++;
    }
    else {
      brightnessNumber = 0;
    }
    EEPROM.write(0, brightnessNumber);
    last_button_time_brightness = button_time_brightness;
  }
}

//pattern bank button interrupt
void bankControl() {
  button_time_bank = millis();

  if (button_time_bank - last_button_time_bank > debounceTime)
  {
    if (bankNumber < numberBanks) {
      bankNumber++;
    }
    else {
      bankNumber = 1;
    }
    last_button_time_bank = button_time_bank;
    patternNumber = 0;

  }
}


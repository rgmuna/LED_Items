\
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, FADE, COLOR_RAIN, BREATHE_COLOR, BREATHE_COLOR_RANDOM, BLOCK_DROP};
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
                case FADE:
                    FadeUpdate();
                    break;
                case COLOR_RAIN:
                    ColorRainUpdate();
                    break;
                case BLOCK_DROP:
                    BlockDropUpdate();
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

int buttonPattern                         = 2; //button #1 for pattern cycling
// int buttonBrightness                      = 3; //button #2 for brightness

volatile int patternNumber                = 1; // keeps track of pattern
// volatile int brightnessNumber             = 255;
volatile int colorLock                    = 0;

//variables to keep track of the timing of recent interrupts
unsigned long button_time_pattern         = 0;
unsigned long last_button_time_pattern    = 0;
unsigned long button_time_brightness      = 0;
unsigned long last_button_time_brightness = 0;
unsigned long button_time_bank            = 0;
unsigned long last_button_time_bank       = 0;

int debounceTime                          = 250; //in milliseconds

// const int POWER_LED_PIN                   = 13; // Output pin for power LED (pin 13 to use Teensy 3.0's onboard LED).
const int NEO_PIXEL_PIN                   = 3; // Output pin for neo pixels.
const int NEO_PIXEL_COUNT                 = 32; // Number of neo pixels. You should be able to increase this without

void pixelsComplete();

NeoPatterns pixels(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800, &pixelsComplete);

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int wait = 80;

////////////// Change these accordingly //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int numberBanks = 3; //set this to the number of banks
int numberBrightnessLevels = 5; //set this to number of bightness levels

int numberPatterns1 = 9;

int brightnessLevels[] = {255, 127, 64, 5, 2, 0}; //Set to maximum brightness
bool randomPattern = false;

int numPatterns[] = {numberPatterns1};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  //pin setups
  pinMode(2, INPUT_PULLUP);     //define pin 2 as input

  pixels.begin(); //start NeoPattern class
  pixels.show();

  pixels.BlockDrop(250, 70);
}

void loop() {
  pixels.Update();

  if (digitalRead(7) == LOW) {
    randomPattern = false;
    patternControl();
  }
}

void pixelsComplete() {
  if (colorLock == 0) {
    pixels.Color1 = pixels.Wheel(random(255));
    pixels.Color2 = pixels.Wheel(random(255));
    pixels.Update();
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
// void patternChoiceBank2() {
//   pixels.CleanPixels();
//   pixels.Index = 0;
//
//   switch(patternNumber) {
//     case 1:
//             pixels.TheaterChase(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 150);
//             break;
//     case 2:
//             pixels.BlockPlacerBoring(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 30);
//             break;
//     case 3:
//             pixels.BlockPlacer(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 30);
//             break;
//     case 4:
//             pixels.BlockDropBoring(pixels.Wheel(random(255)), 70);
//             break;
//     case 5:
//             pixels.BlockDrop(pixels.Wheel(random(255)), 70);
//             break;
//     case 6:
//             pixels.ScannerBoring(pixels.Wheel(random(255)), 40);
//             break;
//     case 7:
//             pixels.Scanner(pixels.Wheel(random(255)), 40);
//             break;
//     case 8:
//             pixels.ColorWipe(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 40);
//             break;
//     case 9:
//             pixels.ColorSwitch(pixels.Wheel(random(255)), pixels.Wheel(random(255)), 40);
//             break;
//     case 10:
//             pixels.RandomFill(pixels.Wheel(random(255)), 70);
//             break;
//     case 11:
//             pixels.ScrollMiddleTwoColor(pixels.Wheel(random(255)), pixels.Wheel(random(255)), random(50, 70));
//             break;
//     case 12:
//             pixels.ScrollEndsTwoColor(pixels.Wheel(random(255)), pixels.Wheel(random(255)), random(50, 70));
//             break;
//     case 13:
//             randomPattern = true;
//             pixels.Update();
//             break;
//   }
// }


////////////////////////////////////////////////////////////////////////////////
//// INTERRUPT SERVICE ROUTINES
///////////////////////////////////////////////////////////////////////////////

////Pattern button interrupt
void patternControl() {
  button_time_pattern = millis();

  if (button_time_pattern - last_button_time_pattern > debounceTime) { //check to see if ISR was called in the last 250 milliseconds
    if (patternNumber < numberPatterns1) {
      patternNumber++;
    } else {
      patternNumber = 1;
    }

   last_button_time_pattern = button_time_pattern;
  }
}

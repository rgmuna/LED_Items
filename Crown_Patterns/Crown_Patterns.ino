\
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pattern types supported:
const char* patterns[] = { "RAINBOW_CYCLE", "EQUALIZER", "RAINBOW_SPIKES", "STACK", "SIDE_FILL", "BREATHE_COLOR", "BLOCK_DROP", "BREATHE_COLOR_RANDOM", "TWINKLE" };

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

const int crownDimensions[8][2] = {
  {0, 21},
  {21, 22},
  {22, 24},
  {24, 27},
  {27, 31},
  {31, 34},
  {34, 36},
  {36, 37}
};

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel {
  public:
    // Member Variables:
    int patternNumber;  // which pattern is running

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern

    // Block Drop variables
    int i;                  // keeps track of number which LEDs to light up
    int Counter1;           // keeps track of which particular LED is going to the bottom
    int Counter2;

    void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }

    // Update each frame of the pattern
    void Update() {
      if ((millis() - lastUpdate) > Interval) {
        lastUpdate = millis();
        switch(patternNumber) {
          case 1:
            RainbowCycleUpdate();
            break;
          case 7:
            BlockDropUpdate();
            break;
          case 6:
            BreatheColorUpdate();
            break;
          case 8:
            BreatheColorUpdate();
            break;
          case 2:
            EqualizerUpdate();
            break;
          case 3:
            RainbowSpikesUpdate();
            break;
//          case "STACK":
//            StackUpdate();
//            break;
//          case "SIDE_FILL":
//            SideFillUpdate();
//            break;
//          case 9:
//            TwinkleUpdate();
//            break;
          default:
            break;
        }
        show();
        Increment();
      }
    }

    // Increment the Index and reset at the end
    void Increment() {
      Index++;
      if (Index >= TotalSteps) {
        Index = 0;
        if (OnComplete != NULL) {
            OnComplete(); // call the completion callback
        }
      }
    }

    // Set point of crown at index "spike" to color "color"
    void setSpikeColor(uint8_t spike, uint32_t color) {
      for (int i=crownDimensions[spike][0]; i<crownDimensions[spike][1]; i++) {
        setPixelColor(i, color);
      }
    }


    // Update pattern as index increases


    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate() {
      for (int i=0; i< numPixels(); i++) {
          setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
      }
    }

    void BlockDropUpdate() {
       if(Index==0){
         //CleanPixels();
         Counter1=0;
         Counter2=0;
       }
      i = Index - Counter1;
      setPixelColor(i, Color1);
      setPixelColor(i-1, 0);

      if(i>numPixels()-2-Counter2){
        Counter1 = Index;
        Counter2++;
      }
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
    }
    
    // Update the Equalizer Pattern
    void EqualizerUpdate() {
      setSpikeColor(0, Wheel(Color1));
      for (int i=1; i<8; i++) {
        if (random(3) < 2) {
          setSpikeColor(i, 0); 
        } else {
          setSpikeColor(i, Wheel(Color1));
        }
      }
    }

    void RainbowSpikesUpdate() {
      for (int i=0; i<8; i++) {
        setSpikeColor(i, Wheel(i * 30 + Index));
      }
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

     uint32_t DimControl(uint32_t color, int dimBright) {
      uint32_t dimColor;
        dimColor = Color(dimBright*(Red(color))/255, dimBright*(Green(color))/255, dimBright*(Blue(color))/255);

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

const int NEO_PIXEL_PIN                   = 3; // Output pin for neo pixels.
const int NEO_PIXEL_COUNT                 = 37; // Number of neo pixels. You should be able to increase this without

const int NumberLoops                     = 2;
int trackLoops                            = 0;

void pixelsComplete();

NeoPatterns pixels(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800, &pixelsComplete);

int numberPatterns =  (sizeof(patterns) / sizeof(char *));

void setup() {
  pixels.begin(); //start NeoPattern class
  pixels.show();
};

void loop() {
  pixels.Update();
};

void pixelsComplete() {
  if (trackLoops < NumberLoops) {
    trackLoops++;
  } else {
    trackLoops = 0;
    patternControl();
  }

  pixels.Update();
};


void patternControl() {
  if (pixels.patternNumber < numberPatterns) {
    pixels.patternNumber++;
  }
  else {
    pixels.patternNumber = 1;
  }

  pixels.Index = 0;
  pixels.Counter1 = 0;
  pixels.Counter2 = 0;
  pixels.i = 0;
  pixels.Interval = 40;
  pixels.TotalSteps = 255;
  pixels.Color1 = pixels.Wheel(random(255));
  pixels.Color2 = pixels.Wheel(random(255));
  pixels.CleanPixels();

  switch(pixels.patternNumber) {
    case 1:
    case 2: 
    case 3: 
      //pixels.ActivePattern = "EQUALIZER";
      break;
//    case 4: 
//      pixels.ActivePattern = "SIDE_FILL";
//      break;
//    case 5: 
//      pixels.ActivePattern = "TWINKLE";
//      break;
    case 6:
      //pixels.ActivePattern = "BREATHE_COLOR";
      pixels.Interval = 5;
      pixels.TotalSteps = 512;
      break;
    case 7:
      //pixels.ActivePattern = "BLOCK_DROP";
      pixels.TotalSteps = (NEO_PIXEL_COUNT*(NEO_PIXEL_COUNT+1))/2;
      pixels.TotalSteps = 512;
      break;
    case 8:
      //pixels.ActivePattern = "BREATHE_COLOR_RANDOM";
      pixels.Interval = 5;
      break;
    case 9:
      //pixels.ActivePattern = "STACK";
      break;
  }
};


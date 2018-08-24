\
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, COLOR_RAIN, BLOCK_DROP, BLOCK_PLACER, COLOR_WIPE, COLOR_SWITCH};
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
class NeoPatterns : public Adafruit_NeoPixel {
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
    int j;
    int Counter;
    int Counter2;
    int RainSize;
    int RainSpacing;

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
        switch(ActivePattern) {
          case RAINBOW_CYCLE:
            RainbowCycleUpdate();
            break;
          case BLOCK_PLACER:
            BlockPlacerUpdate();
            break;
          case COLOR_RAIN:
            ColorRainUpdate();
            break;
          case BLOCK_DROP:
            BlockDropUpdate();
            break;
          case COLOR_WIPE:
            ColorWipeUpdate();
          case COLOR_SWITCH:
            ColorSwitchUpdate();
          default:
              break;
        }
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
///////////////////////////////////////////////////////////////////
//////////////////       PATTERNS         /////////////////////////
///////////////////////////////////////////////////////////////////

//-------------------- RAINBOW CYCLE---------------------------------------------------------
    void RainbowCycle(uint8_t interval, direction dir = FORWARD) {
      ActivePattern = RAINBOW_CYCLE;
      Interval      = interval;
      TotalSteps    = 255;
      Index         = 0;
    }

    void RainbowCycleUpdate() {
      for (int i=0; i< numPixels(); i++) {
          setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
      }
      show();
      Increment();
    }

//-------------------- BLOCK DROP---------------------------------------------------------
    void BlockDrop(uint32_t color1, uint8_t interval) {
      ActivePattern = BLOCK_DROP;
      Interval      = interval;
      TotalSteps    = (numPixels()*(numPixels()+1))/2;
      Color1        = color1;
      Index         = 0;
      i             = 0; //keeps track of number which LEDs to light up
      Counter       = 0; //keeps track of which particular LED is going to the bottom
      Counter2      = 0;
    }

    void BlockDropUpdate() {
     if(Index==0){
       Counter  = 0;
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

//-------------------- COLOR RAIN--------------------------------------------------------- roq
    void ColorRain(uint32_t color1, uint32_t color2,  uint8_t interval) {
      ActivePattern = COLOR_RAIN;
      Interval = interval;
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      RainSize = 4;
      RainSpacing = 15;
      TotalSteps = numPixels() + RainSize * 2 + RainSpacing;
    }

    void ColorRainUpdate() {
        for (i=0; i<=RainSize; i++) {
          setPixelColor(Index + i - RainSize , Color1);
          setPixelColor(Index+i-RainSize-RainSize,  DimControl(Color2, 20));
        }
        if ((Index%(RainSize + RainSpacing)) <(RainSize + RainSpacing)) {
          setPixelColor(Index + i - RainSize-(RainSize + RainSpacing) , Color1);
          setPixelColor(Index+i-RainSize-RainSize-(RainSize + RainSpacing),  DimControl(Color2, 20));
        }

      show();
      Increment();
    }


//-------------------- BLOCK PLACER---------------------------------------------------------
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
        Counter2 = numPixels(); //keeps track of how many pixels on bottom
        Counter  = 0; //keeps track of how many pixels on top
        i        = 0; //keeps track of whether blocks are going up or down
        Index    = TotalSteps-1;
      } else {
        if (i==0) { //blocks going down
          if (Index+Counter != Counter2) {
            setPixelColor(Counter+Index, Color1);
            setPixelColor(Counter+Index-1, 0);

            for(int j=0; j<Counter; j++){
              setPixelColor(j, Color1);
            }
          } else {
            i     = 1;
            Counter2--;
            Index = 0;
          }
        }

        if (i==1){  //blocks going up
          if (Counter2-Index != Counter) {
            setPixelColor(Counter2-Index, Color1);
            setPixelColor(Counter2-Index+1, 0);

            for (int j=Counter2; j<numPixels(); j++) {
                setPixelColor(j, Color1);
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

//-------------------- BLOCK PLACER---------------------------------------------------------
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

//-------------------- HELPER FUNCTIONS ---------------------------------------------------------
    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color) {
      return (color >> 16) & 0xFF;
    }
    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color) {
      return (color >> 8) & 0xFF;
    }\
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

volatile int colorLock                    = 0;
volatile int patternNumber                = 1; // keeps track of pattern


//variables to keep track of the timing of recent interrupts
unsigned long button_time_pattern         = 0;
unsigned long last_button_time_pattern    = 0;
unsigned long button_time_brightness      = 0;
unsigned long last_button_time_brightness = 0;
unsigned long button_time_bank            = 0;
unsigned long last_button_time_bank       = 0;

int debounceTime                          = 250; //in milliseconds

// const int POWER_LED_PIN                = 13; // Output pin for power LED (pin 13 to use Teensy 3.0's onboard LED).
const int NEO_PIXEL_PIN_LEFT              = 3; // Output pin for neo pixels.
const int NEO_PIXEL_PIN_RIGHT             = 22; // Output pin for neo pixels.
const int NEO_PIXEL_COUNT                 = 21; // Number of neo pixels. You should be able to increase this without

const int NumberLoops                     = 5;
int trackLoops                            = 0;

void pixelsComplete();

NeoPatterns left_strand(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_LEFT, NEO_GRB + NEO_KHZ800, &pixelsComplete);
NeoPatterns right_strand(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_RIGHT, NEO_GRB + NEO_KHZ800, &pixelsComplete);

////////////// Change these accordingly //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int numberPatterns = 12;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(38400);
  left_strand.begin(); //start NeoPattern class
  left_strand.show();

  left_strand.BlockPlacer(left_strand.Wheel(random(255)),left_strand.Wheel(random(255)), 20);

  right_strand.begin(); //start NeoPattern class
  right_strand.show();

  right_strand.BlockPlacer(right_strand.Wheel(random(255)),right_strand.Wheel(random(255)), 20);
};

void loop() {
  left_strand.Update();
  right_strand.Update();
};

void pixelsComplete() {
  if (trackLoops < NumberLoops) {
    trackLoops++;
    patternControl();
  } else {
    trackLoops = 0;
     if (patternNumber <= numberPatterns) {
      patternNumber++;
     }
     else {
       patternNumber = 1;
     }
     patternControl();
  }

  left_strand.Update();
  right_strand.Update();
};


void patternControl() {
  left_strand.Index = 0;
  right_strand.Index = 0;

// NONE, RAINBOW_CYCLE, COLOR_RAIN, BLOCK_DROP, BLOCK_PLACER, THEATER_CHASE, COLOR_WIPE, COLOR_SWITCH
  switch(patternNumber) {
    case 1:
            left_strand.BlockPlacer(left_strand.Wheel(random(255)),left_strand.Wheel(random(255)), 20);
            right_strand.BlockPlacer(right_strand.Wheel(random(255)),right_strand.Wheel(random(255)), 20);
            break;
    case 2:
            left_strand.ColorWipe(left_strand.Wheel(random(255)), left_strand.Wheel(random(255)), 50);
            right_strand.ColorWipe(right_strand.Wheel(random(255)), right_strand.Wheel(random(255)), 50);
            break;
    case 3:
            left_strand.ColorWipe(left_strand.Wheel(random(255)), left_strand.Wheel(random(255)), 40);
            right_strand.ColorWipe(right_strand.Wheel(random(255)), right_strand.Wheel(random(255)), 40);
            break;
    case 4:
            left_strand.ColorRain(left_strand.Wheel(255), left_strand.Wheel(255), 30);
            right_strand.ColorRain(right_strand.Wheel(255), right_strand.Wheel(255), 30);
            break;
    case 5:
            left_strand.ColorRain(left_strand.Wheel(70), left_strand.Wheel(70), 40);
            right_strand.ColorRain(right_strand.Wheel(70), right_strand.Wheel(70), 40);
            break;
    case 6:
            left_strand.ColorRain(left_strand.Wheel(170), left_strand.Wheel(170), 50);
            right_strand.ColorRain(right_strand.Wheel(170), right_strand.Wheel(170), 50);
            break;
    case 7:
            left_strand.ColorRain(left_strand.Wheel(random(255)), left_strand.Wheel(random(255)), 60);
            right_strand.ColorRain(right_strand.Wheel(random(255)), right_strand.Wheel(random(255)), 60);
            break;
    case 8:
            left_strand.BlockDrop(left_strand.Wheel(random(255)), 40);
            right_strand.BlockDrop(right_strand.Wheel(random(255)), 40);
            break;
    case 9:
            left_strand.ColorSwitch(left_strand.Wheel(random(255)), left_strand.Wheel(random(255)), 80);
            right_strand.ColorSwitch(right_strand.Wheel(random(255)), right_strand.Wheel(random(255)),80);
            break;
    case 10:
            left_strand.ColorSwitch(left_strand.Wheel(random(255)), left_strand.Wheel(random(255)), 80);
            right_strand.ColorSwitch(right_strand.Wheel(random(255)), right_strand.Wheel(random(255)),80);
            break;
    case 11:
            left_strand.ColorSwitch(left_strand.Wheel(random(255)), left_strand.Wheel(random(255)), 80);
            right_strand.ColorSwitch(right_strand.Wheel(random(255)), right_strand.Wheel(random(255)),80);
            break;
    case 12:
            left_strand.RainbowCycle(40);
            right_strand.RainbowCycle(40);
            break;
  }
};


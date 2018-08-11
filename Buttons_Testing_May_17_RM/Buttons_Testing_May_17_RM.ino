
#include <Adafruit_NeoPixel.h>

// Pattern types supported:
enum  pattern { 
  NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, RANDOM_PIXELS, 
  FLASH_COLOR, SCROLL_COLOR, SCROLL_TWO_COLOR, SCROLL_MIDDLE, SCROLL_MIDDLE_TWO_COLOR, 
  SCROLL_ENDS, SCROLL_ENDS_TWO_COLOR };
// Patern directions supported:
enum  direction { 
  FORWARD, REVERSE };


// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : 
public Adafruit_NeoPixel
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

  void (*OnComplete)();  // Callback on completion of pattern

  // Constructor - calls base-class constructor to initialize strip
  NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
:
    Adafruit_NeoPixel(pixels, pin, type)
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
        //                case RAINBOW_CYCLE:
        //                    RainbowCycleUpdate();
        //                    break;
        //                case THEATER_CHASE:
        //                    TheaterChaseUpdate();
        //                    break;
      case COLOR_WIPE:
                        ColorWipeUpdate();
                        break;
        //                case SCANNER:
        //                    ScannerUpdate();
        //                    break;
        //                case FADE:
        //                    FadeUpdate();
        //                    break;
        //                case RANDOM_PIXELS:
        //                    RandomPixelsUpdate();
        //                    break;
        //                case FLASH_COLOR:
        //                    FlashColorUpdate();
        //                    break;
        //                case SCROLL_COLOR:
        //                    ScrollColorUpdate();
        //                    break;
        //                case SCROLL_TWO_COLOR:
        //                    ScrollTwoColorUpdate();
        //                    break;
        //                case SCROLL_MIDDLE:
        //                    ScrollMiddleUpdate();
        //                    break;
        //                case SCROLL_MIDDLE_TWO_COLOR:
        //                    ScrollMiddleTwoColorUpdate();
        //                    break;
        //                case SCROLL_ENDS:
        //                    ScrollEndsUpdate();
        //                    break;
        //                case SCROLL_ENDS_TWO_COLOR:
        //                    ScrollEndsTwoColorUpdate();
        //                    break;
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
          OnComplete(); // call the comlpetion callback
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

  
  // Initialize for a ColorWipe
  void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
  {
    ActivePattern = COLOR_WIPE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color;
    Index = 0;
    Direction = dir;
  }

  // Update the Color Wipe Pattern
  void ColorWipeUpdate()
  {
    if(Index == 0){
      CleanPixels();
    }

    setPixelColor(Index, Color1);
    show();
    Increment();
  }

 

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Custom Patterns Begin
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

 //Random Pixels
  void RandomPixels(int interval, int steps) {
    ActivePattern = RANDOM_PIXELS;
    Interval = interval;
    TotalSteps = steps;
  }

  // Update the Random Pixels pattern
//  void RandomPixelsUpdate() {
//    setPixelColor(random(numPixels()-1), Wheel(random(255)));
//    setPixelColor(random(numPixels()-1), 0);
//    show();
//    Increment();
//  }

  // Returns the Red component of a 32-bit color
  uint8_t Red(uint32_t color)
  {
    return (color >> 16) & 0xFF;
  }

  // Returns the Green component of a 32-bit color
  uint8_t Green(uint32_t color)
  {
    return (color >> 8) & 0xFF;
  }

  // Returns the Blue component of a 32-bit color
  uint8_t Blue(uint32_t color)
  {
    return color & 0xFF;
  }

  // Return color, dimmed by 75% (used by scanner)
  uint32_t DimColor(uint32_t color)
  {
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
  }

  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
//  uint32_t Wheel(byte WheelPos)
//  {
//    WheelPos = 255 - WheelPos;
//    if(WheelPos < 85)
//    {
//      return Color(255 - WheelPos * 3, 0, WheelPos * 3);
//    }
//    else if(WheelPos < 170)
//    {
//      WheelPos -= 85;
//      return Color(0, WheelPos * 3, 255 - WheelPos * 3);
//    }
//    else
//    {
//      WheelPos -= 170;
//      return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//    }
//  }

  // Reverse direction of the pattern
//  void Reverse()
//  {
//    if (Direction == FORWARD)
//    {
//      Direction = REVERSE;
//      Index = TotalSteps-1;
//    }
//    else
//    {
//      Direction = FORWARD;
//      Index = 0;
//    }
//  }

  // Set all pixels to a color (synchronously)
//  void ColorSet(uint32_t color)
//  {
//    for (int i = 0; i < numPixels(); i++)
//    {
//      setPixelColor(i, color);
//    }
//    show();
//  }

  void CleanPixels() 
  {
    for (int i = 0; i < numPixels(); i++)
    {
      setPixelColor(i, 0);
    }
    show();
  }
};

///////////////////// end of NeoPattern class /////////////////////////
///////////////////////////////////////////////////////////////////////

int buttonPattern = 7; //button #1 for pattern cycling
int buttonBrightness = 8; //button #2 for brightness
int buttonBank = 9; //button #3 for changing pattern bank

volatile int patternNumber = 1; // keeps track of pattern
volatile int brightnessNumber = 1; //keeps track of brightness level
volatile int bankNumber = 1; //keeps track of both buttons being pressed

//variables to keep track of the timing of recent interrupts
unsigned long button_time_pattern = 0;  
unsigned long last_button_time_pattern = 0; 
unsigned long button_time_brightness = 0;  
unsigned long last_button_time_brightness = 0; 
unsigned long button_time_bank = 0;  
unsigned long last_button_time_bank = 0; 

int debounceTime = 250; //in milliseconds

const int NEO_PIXEL_PIN = 3;           // Output pin for neo pixels.
const int NEO_PIXEL_COUNT = 53;         // Number of neo pixels.  You should be able to increase this without
const int POWER_LED_PIN = 13; 

void pixelsComplete(); 

// Define some NeoPatterns for the two rings and the stick
//  as well as some completion routines
NeoPatterns pixels(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_RGB + NEO_KHZ800, &pixelsComplete);

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int wait = 80; 

////////////// Change these accordingly ////////////////////

int numberBanks = 3; //set this to the number of banks
int numberBrightnessLevels = 5; //set this to number of bightness levels

int numberPatterns1 = 3;
int numberPatterns2 = 3;
int numberPatterns3 = 3;

int brightnessLevels[] = {
  255, 191, 127, 64, 0}; //Set to maximum brightness

///////////////////////////////////////////////////////////

void setup() {                
  Serial.begin(38400);  //turn on serial communication
  //pin setups
  pinMode(7, INPUT_PULLUP); //define pin 7 as input
  //digitalWrite(7,HIGH);  // enable pullup resistor
  pinMode(8, INPUT_PULLUP); //define pin 8 as input
  //digitalWrite(8,HIGH);  // enable pullup resistor
  pinMode(9, INPUT_PULLUP); //define pin 9 as input
  //digitalWrite(9,HIGH);  // enable pullup resistor

  // Turn on the power indicator LED.
  //pinMode(POWER_LED_PIN, OUTPUT);
  //digitalWrite(POWER_LED_PIN, HIGH);

  pixels.begin(); //start NeoPattern class


  //pixels.Scanner(pixels.Color(255, 0, 0), 55);
  pixels.RandomPixels(20, 500);

  //  attachInterrupt(buttonPattern, patternControl, FALLING);  //jump to the increment function on falling edge
  //  attachInterrupt(buttonBrightness, brightnessControl, FALLING);  //jump to the increment function on falling edge
  //  attachInterrupt(buttonBank, bankControl, FALLING);  //jump to the increment function on falling edge

}

void loop() {
  pixels.Update();

  if (digitalRead(7) == LOW) {
    patternControl();
    bankChoice();  
  }

  else if (digitalRead(8) == LOW) {

  }

  else if (digitalRead(9) == LOW) {
    bankControl();
    patternControl();
    bankChoice(); 
  }

//  else {
//    pixels.Update();
//  }
  //int brightnessFactor = brightnessLevels[brightnessNumber];


}

void pixelsComplete() {
  pixels.Update();
}

////creates the pixel setting function that incorproates a brightness level
//void setPixelColor( uint16_t n, uint8_t r, uint8_t g, uint8_t b) { 
//   pixels.setPixelColor(n, (brightnessNumber*r/255) , (brightnessNumber*g/255), (brightnessNumber*b/255));
//}

////////////////////////////////////////////////////////////////////////////////
//// PATTERN CALLS 
///////////////////////////////////////////////////////////////////////////////

//Determines what cank you're in and calls the next pattern bank accordingly
void bankChoice() {
  switch(bankNumber) {
  case 1: 
    patternChoiceBank1();
    break;
  case 2: 
    patternChoiceBank2();
    break;
  case 3: 
    patternChoiceBank3();
    break;  
  }
}

void patternChoiceBank1() {
  pixels.CleanPixels();
  pixels.Index = 0;

  switch(patternNumber) {
  case 1: //RED
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(255, 0, 0);
    pixels.Interval = 100;
    pixels.Update();
    break;
  case 2: //GREEN
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(0, 255, 0);
    pixels.Interval = 100;
    pixels.Update();
    break;
  case 3: //BLUE
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(0, 0, 255);
    pixels.Interval = 100;
    pixels.Update();
    break;
  }
}

void patternChoiceBank2() {
  pixels.CleanPixels();
  pixels.Index = 0;

  switch(patternNumber) {
  case 1: //YELLOW
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(255, 255, 0);
    pixels.Interval = 100;
    pixels.Update();
    break;
  case 2: //AQUA
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(0, 255, 255);
    pixels.Interval = 100;
    pixels.Update();
    break;
  case 3: //MAGENTA
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(255, 0, 255);
    pixels.Interval = 100;
    pixels.Update();
    break;
  }
}

void patternChoiceBank3() {
  pixels.CleanPixels();
  pixels.Index = 0;

  switch(patternNumber) {
  case 1: //Slow Purple
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(128, 0, 128);
    pixels.Interval = 50;
    pixels.Update();
    break;
  case 2: //Slow Pink
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(64, 224, 208);
    pixels.Interval = 50;
    pixels.Update();
    break;
  case 3: //Slow Chocolate
    pixels.CleanPixels();
    pixels.ActivePattern = COLOR_WIPE;
    pixels.Color1 = pixels.Color(210, 105, 30);
    pixels.Interval = 50;
    pixels.Update();
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//// Button Presses
///////////////////////////////////////////////////////////////////////////////
//
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
  //  button_time_brightness = millis();
  //
  //  if (button_time_brightness - last_button_time_brightness > debounceTime)
  //  {
  if (brightnessNumber < numberBrightnessLevels) {
    brightnessNumber++;
  }
  else {
    brightnessNumber = 1;
  }
  last_button_time_brightness = button_time_brightness;
  // }
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
    patternNumber = 1;    
  }
}

////////////////////////////////////////////////////////////////////////////////
//// PATTERNS
///////////////////////////////////////////////////////////////////////////////
//
////Bank 1-------------------------------
//
//void colorRed() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 255, 0, 0);
//  }
//  pixels.show();
//}
//
//void colorBlue() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 0, 255);
//  }
//  pixels.show();
//}
//
//void colorGreen() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 255, 0);
//  }
//  pixels.show();
//}
//
//
////Bank 2-------------------------------
//
//void colorRedWalk() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 255, 0, 0);
//    pixels.show();
//    delay(wait);
//  }
//  for (int j=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 0, 0);
//  }
//  pixels.show();
//}
//
//void colorBlueWalk() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 0, 255);
//    pixels.show();
//    delay(wait);
//  }
//  for (int j=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 0, 0);
//  }
//  pixels.show();
//  
//}
//
//void colorGreenWalk() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 255, 0);
//    pixels.show();
//    delay(wait);
//  }
//  for (int j=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 0, 0);
//  }
//  pixels.show();
//}
//
////Bank 3-------------------------------
//
//void colorYellowWalk() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 255, 255, 0);
//    pixels.show();
//    delay(wait);
//  }
//}
//
//void colorPurpleWalk() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 255, 0, 255);
//    pixels.show();
//    delay(wait);
//  }
//}
//
//void colorAquaWalk() {
//  int i;
//  for (i=0; i< NEO_PIXEL_COUNT; i++){
//    pixels.setPixelColor(i, 0, 255, 255);
//    pixels.show();
//    delay(wait);
//  }
//}



//////////////////////////////////////////// EXTRA SHIT//////////////////////

/* serial monitor shit
 delay(1000); 
 Serial.print("Pattern: ");
 Serial.print(patternNumber);
 Serial.print("  Brightness Level: "); 
 Serial.print(brightnessNumber);
 Serial.print("   Bank Number:  ");
 Serial.print(bankNumber);
 Serial.print('\n'); */


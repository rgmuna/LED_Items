
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

////////////// Change these accordingly ////////////////////

int numberBanks = 3; //set this to the number of banks
int numberBrightnessLevels = 5; //set this to number of bightness levels
int numberPatterns1 = 10;
int numberPatterns2 = 5;
int numberPatterns3 = 3;

///////////////////////////////////////////////////////////

void setup() {                
 
  pinMode(7, INPUT); //define pin 7 as input
  digitalWrite(7,HIGH);  // enable pullup resistor
  pinMode(8, INPUT); //define pin 8 as input
  digitalWrite(8,HIGH);  // enable pullup resistor
    pinMode(9, INPUT); //define pin 7 as input
  digitalWrite(9,HIGH);  // enable pullup resistor
  attachInterrupt(buttonPattern, patternControl, FALLING);  //jump to the increment function on falling edge
  attachInterrupt(buttonBrightness, brightnessControl, FALLING);  //jump to the increment function on falling edge
  attachInterrupt(buttonBank, bankControl, FALLING);  //jump to the increment function on falling edge
  Serial.begin(9600);  //turn on serial communication
}

void loop() {
  delay(1000); 
  Serial.print("Pattern: ");
  Serial.print(patternNumber);
  Serial.print("  Brightness Level: "); 
  Serial.print(brightnessNumber);
  Serial.print("   Bank Number:  ");
  Serial.print(bankNumber);
  Serial.print('\n');
}

//////////////////// INTERRUPT SERVICE ROUTINES ///////////////////////////////

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

void brightnessControl() {
  button_time_brightness = millis();

  if (button_time_brightness - last_button_time_brightness > debounceTime)
  {
    if (brightnessNumber < numberBrightnessLevels) {
      brightnessNumber++;
    }
    else {
      brightnessNumber = 1;
    }
    last_button_time_brightness = button_time_brightness;
  }
}

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
  }
}



int ledPin = 13; // LED is attached to digital pin 13
int buttonInt = 7; //button interrupt connectd to pin 7

volatile int x = 0; // variable to be updated by the interrupt

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

void setup() {                
 
  pinMode(7, INPUT);
  //jump to the increment function on falling edge
  attachInterrupt(buttonInt, increment, FALLING);
  Serial.begin(9600);  //turn on serial communication
}

void loop() {
  digitalWrite(ledPin, LOW);
  delay(1000); //pretend to be doing something useful
  Serial.println(x, DEC); //print x to serial monitor
}

// Interrupt service routine for interrupt 0
void increment() {
    x++;
    digitalWrite(ledPin, HIGH);
    last_button_time = button_time;

}

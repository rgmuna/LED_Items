int patternNumber = 0;

void setup()   {                
  Serial.begin(38400);
  pinMode(8, INPUT);
}

void loop()                     
{
  if (digitalRead(8) == HIGH) {
    Serial.println("Button is not pressed...");
  } else {
    Serial.println("Button pressed!!!");
  }
  delay(250);
}


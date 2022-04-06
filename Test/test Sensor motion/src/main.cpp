#include <Arduino.h>

int pin = 15;

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(9600);

  delay(600);
}

void loop() {
  bool isDetected = digitalRead(pin);

  if(isDetected == 1){
    Serial.println("Présence détectée");
  }
  else{
    Serial.println("Présence non détectée");
  }

  delay(100);
}
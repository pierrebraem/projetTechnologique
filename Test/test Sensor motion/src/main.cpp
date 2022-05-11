#include <Arduino.h>

//Numéro du pin auquel le détecteur est branché 
int Inputpin = 15;

void setup() {
  //Définie le Inputpin en mode "INPUT"
  pinMode(Inputpin, INPUT);
  Serial.begin(9600);

  //Temps nécessaire (en minisecondes) pour laisser le capteur faire son calibrage
  Serial.println("Calibrage en cours");
  delay(12000);
}

void loop() {
  //Va stocker le statut de du capteur (1 = détecté, 0 = non détecté)
  bool isDetected = digitalRead(Inputpin);

  //Si la variable est égal à 1, afficher qu'il y a un mouvement
  if(isDetected == 1){
    Serial.println("Présence détectée");
  }
  //Sinon, afficher qu'il n'y a pas de mouvement
  else{
    Serial.println("Présence non détectée");
  }

  delay(100);
}
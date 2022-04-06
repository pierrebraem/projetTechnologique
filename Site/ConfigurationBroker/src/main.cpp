#include <Arduino.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include <HTTPClient.h>
#include <WiFiManager.h>
#include "myFunctions.cpp"
WiFiManager vm;
#define WEBSERVER_H;

#include "MyServer.h";
MyServer *myServer = NULL;

#include <PubSubClient.h>
#include <WiFi.h>
WiFiClient askClient;
PubSubClient client(askClient);

String adresseip;
int port;

int pinSensor = 15;
bool connected = false;

int seconde = 0;
int minute = 0;
int heure  = 0;

const char *SSID = "pierre_";
const char *PASSWORD = "Pierre_";

const char *mqttUser = "";
const char *mqttPassword = "";

std::string CallBackMessageListener(string message){
  while(replaceAll(message, std::string("  "), std::string(" ")));
  string actionToDo = getValue(message, ' ', 0);
  string arg1 = getValue(message, ' ', 1);
  string arg2 = getValue(message, ' ', 2);

  if(string(actionToDo.c_str()).compare(string("adresseipport")) == 0){
    char adresseip[arg1.length() + 1];
    strcpy(adresseip, arg1.c_str());
    int port = atoi(arg2.c_str());

    client.setServer(adresseip, port);
    while(!client.connected()){
      Serial.println("connection au MQTT...");
      if(client.connect("ESP32Client", mqttUser, mqttPassword)){
        //Serial.println("Connecté au MQTT");
        connected = true;
        return String("Connecté au MQTT").c_str();
      }
      else{
        //Serial.println("Échec de connection au MQTT");
        return String("Échec de connection au MQTT").c_str();
        delay(2000);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(pinSensor, INPUT);

  String ssIDRandom, PASSRandom;
  String stringRandom;
  stringRandom = get_random_string(4).c_str();
  ssIDRandom = SSID;
  ssIDRandom = ssIDRandom + stringRandom;
  stringRandom = get_random_string(4).c_str();
  PASSRandom = PASSWORD;
  PASSRandom = PASSRandom + stringRandom;

  if(!vm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
    Serial.println("Erreur de connexion");
  }
  else{
    Serial.println("Connexion réussi");
  }

  myServer = new MyServer(80);
  myServer->initAllRoutes();
  myServer->initCallback(&CallBackMessageListener);
}

void loop() {
  if(connected){
    bool isDetected = digitalRead(pinSensor);

    if(isDetected){
      Serial.println("Début d'une présence détectée");
      client.publish("esp/presence", "Début d'une présence détectée");
      //seconde = seconde + 1;
    }
    else{
      //if(seconde > 59){
        //if(seconde > 3599){
          //heure = seconde / 3600;
          //seconde = seconde - 3600 * heure;
        //}
        //minute = seconde / 60;
        //seconde = seconde - 60 * heure;

        string message = "Fin d'une présence détectée: ";
        //message.append(std::stoi(heure)).append(":").append(std::stoi(minute)).append("");

        //Serial.println(heure);
        //Serial.println(minute);
        //Serial.println(seconde);
        Serial.println(message.c_str());
        client.publish("esp/presence", message.c_str());
        //seconde = 0;
        //minute = 0;
        //heure  = 0;
      //}
      //else{
        //Serial.println(seconde);
        //Serial.println("Fin d'une présence détectée: ");
        //seconde = 0;
      //}
    }
    delay(10000);
  }
}
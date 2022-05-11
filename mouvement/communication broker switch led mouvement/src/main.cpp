/** Copyright (C) 2022 Pierre Braem
 * All rights reserved.
 *
 * Projet technologique
 * Ecole du Web
 * Cours Projet technologique (c)2022
 * 
 * Description : Ce programme permet d'envoyer le statut du capteur de mouvement au broker afin
 * D'allumer ou d'eteindre une LED.
 * 
    @file     main.cpp
    @author   Pierre Braem
    @version  1.0 2022-05-03
    [env:esp32doit-devkit-v1]
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
Librairie utilisée :
    pubsubclient //Cette librairie permet de gérer la connexion Wifi de l'ESP32.
                  Mais aussi à l'envoie et la réception des données entre le broker et l'ESP32.
Configuration du système :
  Adresse IP du broker : 172.16.226.101:1883
  Capteur de mouvement :
    GPIO : 15 
 * */
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Configuration Wifi
const char *ssid = "EcoleDuWeb2.4g";
const char *password = "EcoleDuWEB";

//MQTT broker config
const char *mqtt_server = "172.16.226.101";
unsigned int mqtt_port = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";

//Port DATA du détecteur de mouvement
int pin_data = 15;

//Déclaration de variable WiFi
WiFiClient askClient;
PubSubClient client(askClient);

//Cette fonction permet d'informer l'utilisateur sur la connexion au MQTT.
void reconnect(){
  //Boucle jusqu'à qu'il se reconnecte
  while(!client.connected()){
    Serial.print("Tentative de connexion au MQTT...");
    //Si la connection a été établie, alors dire à l'utilisateur que la connexion a été réussie.
    if(client.connect("ESP32Client", mqttUser, mqttPassword)){
      Serial.println("Connecté au MQTT");
    }
    //Sinon, afficher le message d'erreur et prévenir l'utilisateur qu'une nouvelle tentative est planifiée.
    else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message [");
  Serial.print(topic);
  Serial.print("]");
  for(int i=0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//Cette fonction permet de publier le message sur le broker.
void Publication(char* message){
 Serial.println("Publication des données vers le broker");
  char mqtt_payload[30] = "";
  snprintf(mqtt_payload, 30, message);
  Serial.print("Publication du message: ");
  Serial.println(mqtt_payload);
  //Publie le message "mqtt_payload" sur le sujet "zigbee2mqtt/0x00124b002342c261/set"
  client.publish("zigbee2mqtt/0x00124b002342c261/set", mqtt_payload);
  Serial.println("Données publiées");
}

void setup() {
  //Connexion au Wifi
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.print("Connection au WiFi : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //Tentative de connexion (tant que l'ESP n'est pas connecté à Internet, alors continuer à se connecter)
  Serial.print("Connexion au WiFi");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connexion réussi");
  //Affiche l'adresse IP
  Serial.println("Adresse IP : ");
  Serial.println(WiFi.localIP());

  //Connection au broker
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //Déclarer le pinDATA en INPUT
  pinMode(pin_data, INPUT);
}

void loop() {
  //Si la connection au client est interrompu alors essayer de se reconnecter
  if(!client.connected())
    reconnect();
  client.loop();

  //Va stocker si il y a un mouvement ou non
  bool PIR_status = digitalRead(pin_data);

  if(PIR_status){
    Publication("{\"state\": \"ON\"}");
  }
  else{
    Publication("{\"state\": \"OFF\"}");
  }

  delay(5000);
}
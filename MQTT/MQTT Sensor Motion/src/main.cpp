#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

//Configuration Wifi
const char *ssid = "MQTTSensorMotion";
const char *password = "MQTTSensorMotion";

//MQTT broker config
const char *mqtt_server = "m11.cloudmqtt.com";
unsigned int mqtt_port = 1883;
const char *mqttUser = "public";
const char *mqttPassword = "ESP32@123";

//Port détecteur de mouvement
//int PIR_data = 15;

WiFiClient askClient;
PubSubClient client(askClient);

void setup() {
  //Connexion au Wifi
  Serial.begin(115200);
  Serial.print("Connection au WiFi : ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Connexion au WiFi");
  }
  Serial.println("");
  Serial.println("Connexion réussi");
  Serial.println("Adresse IP :");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  while(!client.connected()){
    Serial.println("connection au MQTT...");
    if(client.connect("ESP32Client", mqttUser, mqttPassword)){
      Serial.println("Connecté au MQTT");
    }
    else{
      Serial.print("Échec de connection au MQTT");
      Serial.print(client.state());
      delay(2000);
    }
  }

  //pinMode(PIR_data, INPUT);
}

void loop() {
  //Initialisation du Détecteur de mouvement
  /* bool PIR_status = digitalRead(PIR_data);

  if(PIR_status){
    Serial.println("Détecteur de mouvement détecté");
  }
  else{
    Serial.println("Aucun détecteur de mouvement détecté");
  }

  //Test envoie de données au MQTT asksensors.com
  Serial.println("Envoie de données vers AskSensors");
  char mqtt_payload[30] = "";
  snprintf(mqtt_payload, 30, "m1=%ld", PIR_status); */
}
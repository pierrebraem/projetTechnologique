#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

//Configuration Wifi
const char *ssid = "EcoleDuWeb2.4g";
const char *password = "EcoleDuWEB";


//MQTT broker config
const char *mqtt_server = "172.16.208.56";
unsigned int mqtt_port = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";

WiFiClient askClient;
PubSubClient client(askClient);

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
  Serial.println("Adresse IP :");
  Serial.println(WiFi.localIP());

  //Connexion au MQTT
  //Enregistre l'adresse IP et le port du broker
  client.setServer(mqtt_server, mqtt_port);
  while(!client.connected()){
    Serial.println("connection au MQTT...");
    //Essaye de se connecter avec l'identifiant et le mot de passe fourni
    if(client.connect("ESP32Client", mqttUser, mqttPassword)){
      Serial.println("Connecté au MQTT");
    }
    else{
      //En cas d'échec, essayer de se reconnecter au broker
      Serial.print("Échec de connection au MQTT");
      Serial.print(client.state());
      delay(2000);
    }
  }

  //publication d'un sujet
  client.publish("esp/test", "ESP32");

  //S'abonner à un sujet
  client.subscribe("esp/test");
}

void loop() {

}
#include <Arduino.h>
#include "MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
    }

void MyServer::initAllRoutes(){
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/login.html", "text/html");
    });

    this->on("/configuration", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    this->on("/CSS.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/CSS.css", "text/css");
    });

    this->on("/ConfigBroker", HTTP_POST, [](AsyncWebServerRequest *request){
        if(request->hasParam("adresseip", true) && request->hasParam("port", true)){
            String adresseip = request->getParam("adresseip", true)->value();
            String port = request->getParam("port", true)->value();

            String params = "adresseipport " + adresseip + " " + port;

            //Serial.println(params);
            std::string reqMQTT = "";
            if(ptrToCallBackFunction) reqMQTT = (*ptrToCallBackFunction)(params.c_str());
            //Serial.println(String(reqMQTT.c_str()));
            request->send(200, "text/plain", String(reqMQTT.c_str()));
        }
    });

    this->on("/connexion", HTTP_POST, [](AsyncWebServerRequest *request){
        if(request->hasParam("user", true) && request->hasParam("mdp", true)){
            String user = request->getParam("user", true)->value();
            String mdp = request->getParam("mdp", true)->value();

            HTTPClient http;
            http.addHeader("content-type" , "application/x-www-form-urlencoded");
            if(user == "admin" && mdp == "123456"){
                request->send(200, "text/plain", "true");
            }
            else{
                request->send(200, "text/plain", "false");
            }            
        }
    });

    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
    });

    this->begin();
}
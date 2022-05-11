/* Copyright (C) 2022 Joséanne Aubut
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 *  
  @file     myMqtt.hpp
  @author   Joséanne Aubut
  @version  1.2 2022-03-29 
  @description
      Classe qui permet de faire la gestion de la communication du Broker en MQTT

  Historique des versions   
                            Versions  Date          Auteur      Description
                            1.1       2022-03-24    JA          Première version de la classe, communication en tant que publisher seulement
                            1.2       2022-03-29    JA          Ajout de la communication en tant que subscriber

  platform = Raspberry Pi
  OS = Linux
  Langage : C++

  Inspiration
      https://www.disk91.com/2013/technology/programming/mosquitto-c-sample-code-to-publish-message/
**/

#ifndef MYMQTT_H
#define MYMQTT_H

#include <mosquittopp.h>
#include <cstring>
#include <cstdio>

class myMqtt : public mosqpp::mosquittopp
{
  private:
   const char * host;
   const char * id;
   const char * topic;
   int          port;
   int          keepalive;

   void on_connect(int rc);
   void on_disconnect(int rc);
   void on_publish(int mid);
   void on_message(const mosquitto_message* message);

  public:

   myMqtt(const char *id, const char * _topic, const char *host, int port);
   ~myMqtt();
   void send_msg(const char * _message);
   bool receive_msg();
};

#endif

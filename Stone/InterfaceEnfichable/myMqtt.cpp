/* Copyright (C) 2022 Joséanne Aubut
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 * 
 * */

#include <iostream>
#include <cstring>
#include <stdexcept>
#include "mosquittopp.h"
#include "myMqtt.hpp"

using namespace std;

myMqtt::myMqtt(const char *_id, const char *_topic, const char *_host, int _port) : mosquittopp(_id)
{
  mosqpp::lib_init();
  this->keepalive = 60;
  this->id = _id;
  this->port = _port;
  this->host = _host;
  this->topic = _topic;


  if (loop_start() != MOSQ_ERR_SUCCESS) {
        std::cout << "loop_start failed" << std::endl;
  }

  username_pw_set("", "");
  //connect_async(this->host,this->port,this->keepalive);
  connect(this->host, this->port, this->keepalive);

};

myMqtt::~myMqtt()
{
  disconnect();
  loop_stop();
  mosqpp::lib_cleanup();
}

void myMqtt::on_connect(int rc)
{
  if (rc == 0)
  {
    cout << " ##-Connected with Broker-## " << std::endl;
  }
  else
  {
    cout << "##-Unable to Connect Broker-## " << std::endl;
  }
}

void myMqtt::on_disconnect(int rc)
{
  cout << " ##-Disconnected from Broker-## " << rc << std::endl;
}

void myMqtt::on_publish(int mid)
{
  cout << "## - Message published successfully" << mid << std::endl;
}

void myMqtt::on_message(const mosquitto_message* message)
{
    cout << "Subscriber received message of topic: " << message->topic << " Data: " << reinterpret_cast<char*>(message->payload) << "\n";
}


void myMqtt::send_msg(const char * _message)
{
  int ret = publish(NULL, topic, strlen(_message), _message, 1, false);

  if (ret != MOSQ_ERR_SUCCESS) {
        std::cout << "Sending failed." << std::endl;
  }
}

bool myMqtt::receive_msg()
{
  int set = subscribe(NULL, this->topic, 2);
  return set; 
}
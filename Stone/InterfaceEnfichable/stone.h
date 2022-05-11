#ifndef STONE_H
#define STONE_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>

#include "MySerial.h"

struct dataRead
{
    int id; // 0 = pas de données
    char command[80];
    char name[80];
    int type;
    char line[2048];
};

class Stone : public MySerial
{
using MySerial::MySerial;
private:


public:
    Stone(std::string portName);
    ~Stone(){};
    int init(char *comPort,uint speed = 115200);
    dataRead getValidsDatasIfExists();

    void setTexte(const char* label, const char* texte);
    void setButton(const char* label, const char* texte);
};

#endif

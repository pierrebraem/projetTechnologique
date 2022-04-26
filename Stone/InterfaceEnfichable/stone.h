#ifndef STONE_H
#define STONE_H

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "MySerial.h"

struct dataRead
{
    int id; // 0 = pas de données
    char command[80];
    char name[80];
    int type;
    char line[2048];
};

class Stone
{
private:
    MySerial *serialPort;
    int USB;
    char commPortName[80];
    void displayCharString(char *str, int len);

    int formatStrCommand(const char *cmd_code, const char *widget, char *buffer);
    ssize_t read_port(int fd, uint8_t *buffer, size_t size);
    std::string charToString(char str, std::string formatStr);
    std::string intToString(int value, std::string formatStr);

    void setColor(const char *labelName, unsigned int argb, const char *type);

public:
    Stone();
    ~Stone(){};
    int init(char *comPort,uint speed = 115200);
    void changePage(const char *pageName,int timeWaitingAfterInMs = 0);
    int getVersion();
    dataRead getValidsDatasIfExists();
    void sendDatas(const char *dataToSend);
    void setLabel(const char *labelName,const char *value);
    void setEnable(const char *labelName,const bool value);
    void setVisible(const char *labelName,const bool value);

    void setDate(const char *labelName,int annee, int mois,int jour, int heure,int minute,int sec=0);

    void setPosition(const char *labelName,int posX,int posY);
    void setColorBackground(const char *labelName,unsigned int r,unsigned int g,unsigned int b,unsigned int alpha);
};

#endif

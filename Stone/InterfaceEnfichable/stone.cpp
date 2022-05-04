#include "stone.h"
#include <thread>

Stone::Stone(std::string portName):MySerial{portName}{};

void Stone::setTexte(const char* label, const char* texte){
    char cmd[99];
    strcpy(cmd, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"");
    strcpy(cmd, label);
    strcpy(cmd, "\", \"text\":\"");
    strcpy(cmd, texte);
    strcpy(cmd, "\"}>ET");
    writeIt((char*) cmd);
}


int Stone::init(char *comPort, uint speed){
    MySerial *mySerial = new MySerial(comPort);
    return 0;
}
#include "stone.h"
#include <thread>

Stone::Stone(std::string portName):MySerial{portName}{};

void Stone::setTexte(const char* label, const char* texte){
    std::stringstream ss;
    char cmd[99];
    ss << "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"" << label << "\",\"text\":\"" << texte << "\"}>ET";
    std::string commande = ss.str();
    strcpy(cmd, commande.c_str());
    writeIt((char*) cmd);
}

void Stone::setButton(const char* label, const char* texte){
    std::stringstream ss;
    char cmd[99];
    ss << "ST<{\"cmd_code\":\"set_text\",\"type\":\"button\",\"widget\":\"" << label << "\",\"text\":\"" << texte << "\"}>ET";
    std::string commande = ss.str();
    strcpy(cmd, commande.c_str());
    std::cout << cmd << " \n";
    writeIt((char*) cmd);
}

int Stone::init(char *comPort, uint speed){
    MySerial *mySerial = new MySerial(comPort);
    return 0;
}
#include "stone.h"
#include <thread>

Stone::Stone(std::string portName):MySerial{portName}{};

void Stone::initplugInMouvement(){
    char cmdFormatNom[99];
    strcpy(cmdFormatNom, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"nomdectecteurlabel\",\"text\":\"test\"}>ET");
    std::cout << cmdFormatNom << "\n";
    writeIt((char*) cmdFormatNom);
    /* char cmdFormatDescription[99];
    strcpy(cmdFormatDescription, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"description\",\"text\":\"test\"}>ET");
    std::cout << cmdFormatDescription << "\n"; */
    //serialPort->writeIt((char*) cmdFormatDescription);
}


int Stone::init(char *comPort, uint speed){
    MySerial *mySerial = new MySerial(comPort);
    return 0;
}
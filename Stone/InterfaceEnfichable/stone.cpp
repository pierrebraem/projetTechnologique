#include "stone.h"
#include <thread>

Stone::Stone(){

}

int Stone::init(char *comPort, uint speed){
    MySerial *mySerial = new MySerial(comPort);
    //int resultat = mySerial->init(comPort);
    return 0;
}
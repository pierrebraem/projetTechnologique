//Librement inspiré et adapté du site :
//https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

//sudo apt-get install cmake
//sudo apt-get install git
//git init (dans le répertoire de travail)

//sudo make
/* Exemple
    #include "MySerial.h"
    MySerial *mySerial;

    int main() {
      char ComPortName[] = {"/dev/ttyS2" };
   
        mySerial = new MySerial(ComPortName);

        char cmdFormat2[99];
        strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET"); 
        mySerial->writeIt((char*)cmdFormat2);	

        n = mySerial->readIt( (char*)cmdFormat2, 255);
    }


Outils et références
    https://www.scadacore.com/tools/programming-calculators/online-checksum-calculator/

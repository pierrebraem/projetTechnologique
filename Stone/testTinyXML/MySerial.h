/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 *  
    @file     MySerial.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15 
    @description
      Démonstration comment utiliser le PORT SERIE RS232

    platform = Raspberry Pi
    OS = Linux
    Langage : C++

    Inspiration
        https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart
**/
#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <string>

#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

class MySerial {
    private:
       int uart_filestream;

    public:
        MySerial(std::string portName);
        ~MySerial() { close(uart_filestream);};
        
        int writeIt(std::string cmdFormat2);
        int readIt(char *data, int len);
};
#endif
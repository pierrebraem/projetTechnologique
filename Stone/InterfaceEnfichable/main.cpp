/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe SerialPort

    platform = Raspberry Pi
    OS = Linux
    Langage : C++

    Inspiration
        https://raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

    Compilation (voir le fichier ReadMe.txt pour plus de détails)
      Utiliser le Makefile
        sudo make

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain       Première version du logiciel

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
          Optenir la version du Firmware de l'écran

 * */
#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <string.h>
#include <dlfcn.h>        //Dynamic library

#include <thread> //std::thread
#include <chrono> //Sleep

#include "panelAddon.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "stone.h"

#include "MySerial.h"
MySerial *mySerial;

namespace fs = std::filesystem;

// Menu utilisé pour tester les fonctionalités implantées
int selection = 0;
char data[255];

void* plugIns[1024];
create_t* create_plugIns[1024];
destroy_t* destroy_plugIns[1024];
panelAddon* addon[1024];
int NbrePlugIns = 0;
int NbreAddon = 0;
static Stone stone; //todo

// Structure local utilisés pour garder les informations lues de l'écran
struct datasRead
{
  int id; // Si 0 alors il n'y a pas de données, <0 Numero commande non traitée, >1 Numero de la commande traité
  char command[80];
  char name[80];
  int type;
  char line[2048];
};

void menu()
{
  std::cout << std::endl;
  selection = 0;
  std::cout << " Please choose from the following options - \n";
  std::cout << " 7. rotation. \n";
  std::cout << " 6. xy. \n";
  std::cout << " 5. labelTest. \n";
  std::cout << " 4. SetHeure.\n";
  std::cout << " 3. Reboot.\n";
  std::cout << " 2. Hello.\n";
  std::cout << " 1. Lire la version du FirmWare.\n";
  std::cout << " 0. Exit.\n";
  std::cout << "\t";
  std::cout << "Commande : ";
  std::cin >> selection;

  if (selection == 1)
  { // Demande la version du FirmWare
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  }

  if (selection == 2)
  { // Demande test
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_hello\",\"type\":\"system\"}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  }

  if (selection == 3)
  { // Demande reboot
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_reboot\",\"type\":\"system\"}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  }

  if (selection == 4)
  { // Demande set heure
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_date\",\"type\":\"digit_clock\",\"widget\":\"digit_clock\",\"date\":\"2022-02-09 13:58:30\"}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  }
  if (selection == 5)
  { // Demande set heure
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"label1\",\"text\":\"test\"}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  }
  if (selection == 6)
  { // Demande set heure
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_xy\",\"type\":\"label\",\"widget\":\"label1\",\"x\":150,\"y\":150}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);
    menu();
  }

  if (selection == 7)
  { // Demande set heure
    char cmdFormat2[99];
    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_rotation\",\"type\":\"image\",\"widget\":\"logocegep\",\"rotation\":90}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);

    sleep(200);

    strcpy(cmdFormat2, "ST<{\"cmd_code\":\"set_rotation\",\"type\":\"image\",\"widget\":\"logocegep\",\"rotation\":150}>ET");
    std::cout << cmdFormat2 << "\n";
    mySerial->writeIt((char *)cmdFormat2);

    menu();
  }

  else if (selection == 0)
  {
    return;
  }
  menu();
}

std::string intToHexa(int value)
{
  char buffer[10];
  sprintf(buffer, "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr)
{
  char buffer[10];
  sprintf(buffer, formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr)
{
  char buffer[10];
  sprintf(buffer, formatStr.c_str(), value);
  return (buffer);
};

// Lire le contenu disponible sur le port série
// Retourne les données dans la structure local datasRead
// Si dataRead.id == 0 alors il n'y a pas de données lues sur le port
// Exemple de données qui peuvent être lues sur le port:
//   Basic format(Voir la documentation de l'écran pour plus de détails):
//     Frame head  : ST<
//     Command     : 0x1068 (exemple)
//     Longeur     : 0x0004 (exemple)
//     Data        : ctew   (exemple)
//     Key Value   : 0x01   (exemple)
//     Frame Tail  : >ET
//     CRC         : AB 24  (exemple)
datasRead getValidsDatasIfExists()
{
  int n, ii;
  datasRead rd;

  int commande, longeur;
  char data[2048];
  struct
  {
    union
    { // Commande
      unsigned short hexaShort;
      struct
      {
        char c1;
        char c2;
      } c;
    } shortDataCommand;
    union
    { // Longeur
      unsigned short hexaShort;
      struct
      {
        char c1;
        char c2;
      } c;
    } shortDataLongeur;

  } shortData;

  // Initialisation
  rd.id = 0; // 0 : Pas de données
  rd.line[0] = 0x00;
  rd.command[0] = 0x00;
  rd.name[0] = 0x00;
  rd.type = 0;

  // Essai de trouver un S
  n = mySerial->readIt((char *)data, 1);
  while ((n == 1) && (data[0] != 'S'))
  {
    n = mySerial->readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "S FOUND\n";

  // Essai de trouver un T
  n = mySerial->readIt((char *)data, 1);
  while ((n == 1) && (data[0] != 'T'))
  {
    n = mySerial->readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "T FOUND\n";

  // Essai de trouver un <T>
  n = mySerial->readIt((char *)data, 1);
  while ((n == 1) && (data[0] != '<'))
  {
    n = mySerial->readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "< FOUND\n";

  // Lecture de la commande et de la longeur de la donnee
  n = mySerial->readIt(&shortData.shortDataCommand.c.c1, 4);

  // Inverser certaines données
  std::swap(shortData.shortDataCommand.c.c2, shortData.shortDataCommand.c.c1);
  std::swap(shortData.shortDataLongeur.c.c2, shortData.shortDataLongeur.c.c1);

  commande = shortData.shortDataCommand.hexaShort;
  longeur = shortData.shortDataLongeur.hexaShort;

  // Lecture Data
  n = mySerial->readIt((char *)data, longeur);

  switch (commande)
  {

  case 0x0002:
  { // Version
    int keyValue = (int)data[longeur - 1];
    data[longeur - 1] = 0x00;

    // Lire les données suivantes : TAIL (3 char ">ET") et CRC (Hexa16)
    char TailDatas[5];
    n = mySerial->readIt(TailDatas, 5);
    // Check if TAIL (>ET) is OK
    if ((n != 5) || (TailDatas[0] != '>') || (TailDatas[1] != 'E') || (TailDatas[2] != 'T'))
      return (rd);
    // Nous ne vérifions pas le CRC pour plus de rapidité mais ce serait mieux...
    // Traitement du CRC
    // int crc = TailDatas[4]; crc <<= 8; crc |= TailDatas[3];
    // std::cout << "Crc: " << intToString(crc, "%4X") << "\n";

    rd.id = commande;
    strcpy(rd.command, "Version");
    strcpy(rd.name, data);
    rd.type = keyValue;

    return (rd);
    break;
  };

  default:
  {

    int keyValue = (int)data[longeur - 1];
    data[longeur - 1] = 0x00;
    // std::cout << "Key Value: " << keyValue << "\n";
    // std::cout << "Data: " << data << "\n";

    // Lire les données suivantes : TAIL (3 char ">ET") et CRC (Hexa16)
    char TailDatas[5];
    n = mySerial->readIt(TailDatas, 5);

    // Check if TAIL (>ET) is OK
    if ((n != 5) || (TailDatas[0] != '>') || (TailDatas[1] != 'E') || (TailDatas[2] != 'T'))
      return (rd);
    // Nous ne vérifions pas le CRC pour plus de rapidité mais ce serait mieux...
    // Traitement du CRC
    // int crc = TailDatas[4]; crc <<= 8; crc |= TailDatas[3];
    // std::cout << "Crc: " << intToString(crc, "%4X") << "\n";

    rd.id = -commande;
    strcpy(rd.command, "???????");
    strcpy(rd.name, data);
    rd.type = keyValue;

    return (rd);
    break;
  };
  };

  return (rd);
};

// Thread qui permet de LOOPER et lire le contenu du port serie
// avec l'aide du la fonction getValidsDatasIfExists
void fonctionLoop()
{
  while (true)
  {

    datasRead rd = getValidsDatasIfExists();
    // std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
    switch (rd.id)
    {

      case 0x0002:
      { // Version
        std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
        std::string stoneVersion = rd.name;
        std::cout << "Version : " << stoneVersion.c_str() << "\n";

        // std::this_thread::sleep_for(std::chrono::milliseconds(10));

        break;
      }
/*       case 0x1001:
      { // Bouton
        std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
        std::string stoneVersion = rd.name;
        std::cout << " ok";

        // std::this_thread::sleep_for(std::chrono::milliseconds(10));

        break;
      } */
    }

    if (rd.id < 0){


      std::cout << abs(rd.id);
      std::cout << rd.name;
      std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type << ")\n";
    }

    if (rd.id == 0){
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

  }
}

int main(int argc, char **argv)
{
  using std::cout;
  using std::cerr;

  char serialPort[255];
  strcpy(serialPort, "/dev/");
  strcat(serialPort, argv[1]);

  mySerial = new MySerial(serialPort);

  char ComPortName[] = {"/dev/" };
  int valRet = stone.init(ComPortName, 115200);
  if(valRet==-1){
    return(0);
  }

  //return(0);
  //Lecture des configurations Xml du programme
  //Vide et initialise tous les éléments de l'écran
  rapidxml::file<> xmlFile("stone.xml");
  rapidxml::xml_document<> doc;
  doc.parse<0>(xmlFile.data());
  //rapidxml::xml_node<> *node = doc.first_node();
  std::string xmlVersion = (doc.first_node("version")? doc.first_node("version")->value():"");
  std::string xmlTeam = (doc.first_node("team")? doc.first_node("team")->value():"");
  std::string xmlThanks = (doc.first_node("thanks")? doc.first_node("thanks")->value():"");
  //handlenode(node);



  //Parcourir le répertoire pour loader les libraries
  int iPlugIns = 0;
  std::string path = "./";
  std::string ext(".so");
  //Va lire tous les fichiers du chemin (le chemin est stockée dans path)
  for (const auto & entry : fs::directory_iterator(path)){
    //Si l'extension est égal à la variable ext (ici .so) alors charger la librairie
    if (entry.path().extension() == ext) {
      std::cout << "\nLibrarie trouvee: " << entry.path() << std::endl;

      //Essai de loader la librarie
      // load the plugInFumee library
      std::string  libraryFileName = entry.path();
      plugIns[NbrePlugIns] = dlopen(libraryFileName.c_str(), RTLD_LAZY);
      if (!plugIns[NbrePlugIns] ) {
        cerr << "Cannot load " << libraryFileName.c_str() << ": " << dlerror() << '\n';
        return 1;
      }
      std::cout << "\nLibrarie ouverte " << std::endl;

            // reset errors
            dlerror();

            // load the symbols
            create_plugIns[NbrePlugIns] = (create_t*) dlsym(plugIns[NbrePlugIns] , "create");
            const char* dlsym_error = dlerror();
            if (dlsym_error) {
                cerr << "Cannot load symbol create: " << dlsym_error << '\n';
                return 1;
                }

            destroy_plugIns[NbrePlugIns] = (destroy_t*) dlsym(plugIns[NbrePlugIns], "destroy");
            dlsym_error = dlerror();
            if (dlsym_error) {
                cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
                return 1;
                }

            //Trouver tous les fichiers Xml correspendants (maximum de 10 pour l'instant)
            int vRet = 0;
            for(int jj=0; jj<10; jj++){
                std::string fileName = entry.path().parent_path();
                fileName += std::string("/");
                fileName += entry.path().filename().replace_extension(".xml");
                //fileName += std::to_string(jj);
                
                if(fs::exists(fileName)){
                    cout << "File exist: " << fileName << "\n";

                    addon[NbreAddon] = create_plugIns[NbrePlugIns]();

                    vRet = addon[NbreAddon]->init(fileName, &stone);
                    cout << vRet << "\n";
                    if (vRet < 0 ){
                        cerr << "Initialisation addon failed: " << vRet << '\n';
                        continue;
                        }

                    NbreAddon++;
                    }
                }
              NbrePlugIns++;
            }
          }

    //Parcourir les plugins et les initialiser
    for(int i=0; i<NbreAddon; i++) {
      addon[i]->set_side_length(7);
      cout << "The area is: " << addon[i]->area() << '\n';
    }

    /* for(int i=0; i<NbreAddon; i++){
      cout << "Nombre: " << addon[i]->nombre1() << '\n'; 
    } */

  //setUpStonePanel();

  //Lancer un tread pour lire les données de la tablette Stone
  std::thread first (fonctionLoop);

  menu();


  //Détruire les addOns
  for(int i=0; i<NbreAddon; i++) {
    delete addon[i];
    std::cout << "\nAddOn détruit : " << i << std::endl;
  }

  //Détruire les plugins
  for(int i=0; i<NbrePlugIns; i++) {
    // unload the library
    dlclose(plugIns[i]);
    std::cout << "\nLibrarie détruite : "<< i << std::endl;
  }

  return(0);
}

#include "stone.h"
#include <thread>

Stone::Stone(std::string portName):MySerial{portName}{};

// Lire le contenu disponible sur le port série
// Retourne les données dans la structure local dataRead
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
dataRead Stone::getValidsDatasIfExists()
{
  int n, ii;
  dataRead rd;

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
  n = readIt((char *)data, 1);
  while ((n == 1) && (data[0] != 'S'))
  {
    n = readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "S FOUND\n";

  // Essai de trouver un T
  n = readIt((char *)data, 1);
  while ((n == 1) && (data[0] != 'T'))
  {
    n = readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "T FOUND\n";

  // Essai de trouver un <T>
  n = readIt((char *)data, 1);
  while ((n == 1) && (data[0] != '<'))
  {
    n = readIt((char *)data, 1);
  };
  if (n <= 0)
    return (rd);
  // std::cout << "< FOUND\n";

  // Lecture de la commande et de la longeur de la donnee
  n = readIt(&shortData.shortDataCommand.c.c1, 4);

  // Inverser certaines données
  std::swap(shortData.shortDataCommand.c.c2, shortData.shortDataCommand.c.c1);
  std::swap(shortData.shortDataLongeur.c.c2, shortData.shortDataLongeur.c.c1);

  commande = shortData.shortDataCommand.hexaShort;
  longeur = shortData.shortDataLongeur.hexaShort;

  // Lecture Data
  n = readIt((char *)data, longeur);

  switch (commande)
  {

  case 0x0002:
  { // Version
    int keyValue = (int)data[longeur - 1];
    data[longeur - 1] = 0x00;

    // Lire les données suivantes : TAIL (3 char ">ET") et CRC (Hexa16)
    char TailDatas[5];
    n = readIt(TailDatas, 5);
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
    n = readIt(TailDatas, 5);

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
    writeIt((char*) cmd);
}

int Stone::init(char *comPort, uint speed){
    MySerial *mySerial = new MySerial(comPort);
    return 0;
}
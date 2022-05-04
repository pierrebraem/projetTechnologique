/**
    Classe plugInMouvement : Classe permettant de gérer les commandes du sensor de mouvement sur l'écran Stone
    @file plugInMouvement.cpp
    @author Pierre Braem
    @version 1.0 2022-05-01
    
    Historique des versions   
                            Versions  Date        Auteur      Description
                            1.0       2022-05-01  Pierre      Première version de la classe
                            
                            
    librairies utilisées :
    - myMqtt
    - Rapidxml
    - rapidxml_utils
    - MySerial
    - PanelAddon
    
    Exemple d'utilisation 
        //Définition 
            #include "panelAddon.hpp"
            class plugInMouvement: public panelAddon {
                public:
                
                private:
**/
#include "panelAddon.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "myMqtt.hpp"
#include "stone.h"
#include <cmath>
#include <string>

class plugInMouvement : public panelAddon{
public:
    //Déclaration variable XML
    std::string xmlNom;
    std::string xmlDescription;

    //Déclaration de la classe "myMqtt"
    //class myMqtt *mqtt;

    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }

    //Va afficher le premier détecteur de mouvement dans la liste XML lors de l'affichage de la page
    int init(std::string filename, Stone* stone){
        rapidxml::xml_node<> *root_node;
        rapidxml::file<> xmlFile(filename.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        //Récupère le nom est la description du premier détecteur de mouvement de la liste dans le fichier XML
        root_node = doc.first_node("Mouvements");
        rapidxml::xml_node<> *Mouvement_node = root_node->first_node("Mouvement");
        xmlNom = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
        xmlDescription = Mouvement_node->first_node("Peripherique")->first_node("Description")->value();

        //Met tous les noms des senseurs dans un tableau afin de les afficher dans des boutons
        int i = 0;
        std::string xmlNoms[9];
        for(Mouvement_node; Mouvement_node; Mouvement_node = Mouvement_node->next_sibling()){
            if(i < 9){
                xmlNoms[i] = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
                i++;
            }
        }

        //Met les informations dans l'écran Stone 
        stone->setTexte("nomdectecteurlabel", xmlNom.c_str());
        stone->setTexte("descriptioncadre", xmlDescription.c_str());

        /* std::cout << xmlNom << "\n";
        std::cout << xmlDescription << "\n";
        for(std::string x : xmlNoms){
            std::cout << x << "\n";
        } */

        //Récupère l'historique de la BDD

        return 0;
    }

    /* void testMQTT(){
        myMqtt *mqtt = new myMqtt("test", "rasp/test", "172.16.226.101", 1883);
        mqtt->send_msg("Hello");
        while(1){
            if(mqtt->receive_msg() == 1){
                //std::cout << "Message reçu du broker \n";
            }

            sleep(1);
        }
        delete mqtt;
    } */

    //Récupère du MQTT le statut actuel du capteur
    /* void lireMouvement(){
        mqtt = new myMqtt("mouvement", "zigbee2mqtt/0x00124b002342c261/get", "172.16.226.101", 1883);
        bool set = mqtt->receive_msg();
        if(set == false){
            char cmdFormatOn[99];
            strcpy(cmdFormatOn, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"lecturecadre\",\"text\":\"ON\"}>ET");
            mySerial->writeIt((char* ) cmdFormatOn);
        }
        else{
            char cmdFormatOff[99];
            strcpy(cmdFormatOff, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"lecturecadre\",\"text\":\"OFF\"}>ET");
            mySerial->writeIt((char* ) cmdFormatOff);
        }
        delete mqtt;
    } */

    //Charge le capteur de mouvement sélectionner
    /* void ChargerMov(const char* mov, std::string filename, Stone* stone){
        //Voir ce que la tablette retourne lorsqu'on appuie sur un bouton
        std::string idMov; //Variable temporaire
        rapidxml::xml_node<> *root_node;
        rapidxml::file<> xmlFile(filename.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        root_node = doc.first_node("Mouvements");
        rapidxml::xml_node<> *Mouvement_node = root_node->first_node("Mouvement");
        for(Mouvement_node; Mouvement_node; Mouvement_node = Mouvement_node->next_sibling()){
            if(Mouvement_node->first_attribute("id")->value == idMov){
                xmlNom = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
                xmlDescription = Mouvement_node->first_node("Peripherique")->first_node("Description")->value();
                char cmdFormatNom[99];
                strcpy(cmdFormatNom, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"description\",\"text\":\"");
                strcpy(cmdFormatNom, xmlNom);
                strcpy(cmdFormatNom, "\"}>ET");
                mySerial->writeIt((char*) cmdFormatNom));
                char cmdFormatDescription[99];
                strcpy(cmdFormatDescription, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"description\",\"text\":\"");
                strcpy(cmdFormatDescription, xmlDescription);
                strcpy(cmdFormatDescription, "\"}>ET");
                mySerial->writeIt((char*) cmdFormatDescription);
            }
        }

        //Récupère l'historique de la base de données
    } */
};

extern "C" panelAddon* create(){
    return new plugInMouvement;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} 
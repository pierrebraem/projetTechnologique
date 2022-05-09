/**
    Classe plugInLumiere : Classe permettant de gérer les commandes de la lumière sur l'écran Stone
    @file plugInLumiere.cpp
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
            class plugInLumiere: public panelAddon {
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
#include <sstream>

class plugInLumiere : public panelAddon{
    public:
    //Déclaration variable XML
    std::string xmlNom;
    std::string xmlDescription;
    char* xmlNoms[9];

    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }
    
    //Va afficher la première lumière dans la liste XML lors de l'affichage de la page
    int init(std::string filename, Stone* stone){
        rapidxml::xml_node<> *root_node;
        rapidxml::file<> xmlFile(filename.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        //Récupère le nom est la description de la première lumière de la liste dans le fichier XML
        root_node = doc.first_node("Lumieres");
        rapidxml::xml_node<>* Lumiere_node = root_node->first_node("Lumiere");
        xmlNom = Lumiere_node->first_node("Peripherique")->first_node("Nom")->value();
        xmlDescription = Lumiere_node->first_node("Peripherique")->first_node("Description")->value();

        //Met tous les noms des senseurs dans un tableau afin de les afficher dans des boutons
        int i = 0;
        for(Lumiere_node; Lumiere_node; Lumiere_node = Lumiere_node->next_sibling()){
            if(i < 9){
                xmlNoms[i] = Lumiere_node->first_node("Peripherique")->first_node("Nom")->value();
                i++;
            }
        }

       //Met les informations dans l'écran Stone 
        stone->setTexte("nomlectecteurlabel2", xmlNom.c_str());
        stone->setTexte("descriptioncadre2", xmlDescription.c_str());

        for(int j = 0; i > j; j++){
            int number = j + 1;
            std::stringstream strs;
            std::string s;
            strs << number;
            std::string ch = strs.str();

            s += "bouton_lamp";
            s += ch;
            stone->setButton(s.c_str(), xmlNoms[j]);
        }


        //Récupère l'historique de la BDD

        return 0;
    }
};

extern "C" panelAddon* create(){
    return new plugInLumiere;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} 

    /* void ChargerLamp(const char* lamp, std::string filename, Stone* stone){
        //Voir ce que la tablette retourne lorsqu'on appuie sur un bouton
        std::string idlamp; //Variable temporaire
        rapidxml::xml_node<> *root_node;
        rapidxml::file<> xmlFile(filename.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        root_node = doc.first_node("Lumieres");
        rapidxml::xml_node<> *Mouvement_node = root_node->first_node("Lumiere");
        for(Mouvement_node; Mouvement_node; Mouvement_node = Mouvement_node->next_sibling()){
            if(Mouvement_node->first_attribute("id")->value == idlamp){
                xmlNom = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
                xmlDescription = Mouvement_node->first_node("Peripherique")->first_node("Description")->value();
                char cmdFormatNom[99];
                strcpy(cmdFormatNom, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"description2\",\"text\":\"");
                strcpy(cmdFormatNom, xmlNom);
                strcpy(cmdFormatNom, "\"}>ET");
                mySerial->writeIt((char*) cmdFormatNom));
                char cmdFormatDescription[99];
                strcpy(cmdFormatDescription, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"description2\",\"text\":\"");
                strcpy(cmdFormatDescription, xmlDescription);
                strcpy(cmdFormatDescription, "\"}>ET");
                mySerial->writeIt((char*) cmdFormatDescription);
            }
        }

        //Récupère l'historique de la base de données
    } */
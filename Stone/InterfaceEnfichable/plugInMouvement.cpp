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
    - Curl
    - Json
    
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
#include <sstream>
#include <curl/curl.h>

class plugInMouvement : public panelAddon{
public:
    //Déclaration variables XML
    std::string xmlNom;
    std::string xmlDescription;
    char* xmlNoms[9];

    //Déclaration variable mqtt
    myMqtt* mqtt = new myMqtt("mouvement", "zigbee2mqtt/0x00124b002342c261", "172.16.226.101", 1883);

    //Déclaration variables curl
    CURL* curl;
    CURLcode res;
    std::string historique;

    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }

    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp){
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
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
        for(Mouvement_node; Mouvement_node; Mouvement_node = Mouvement_node->next_sibling()){
            if(i < 9){
                xmlNoms[i] = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
                i++;
            }
        }

        //Met les informations dans l'écran Stone 
        stone->setTexte("nomdectecteurlabel", xmlNom.c_str());
        stone->setTexte("descriptioncadre", xmlDescription.c_str());

        for(int j = 0; i > j; j++){
            int number = j + 1;
            std::stringstream strs;
            std::string s;
            strs << number;
            std::string ch = strs.str();

            s += "bouton_mov";
            s += ch;
            stone->setButton(s.c_str(), xmlNoms[j]);
        }

        //Récupère l'historique de la BDD
        curl = curl_easy_init();
        if(curl){
            curl_easy_setopt(curl, CURLOPT_URL, "http://172.16.209.112:3000/api/presence/Mov03");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &historique);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            std::cout << historique << " \n";
        }

        return 0;
    }

    //Récupère du MQTT le statut actuel du capteur
    void lireMouvement(){
        bool set = mqtt->receive_msg();
        if(set == 0){
            
        }
        else{
            std::cout << "une erreur s'est produite" << " \n";
        }
    }

    //Charge le capteur de mouvement sélectionner
    void ChangerMov(char* mov, std::string filename, Stone* stone){
        std::cout << mov[10] << " \n";
    }
};

extern "C" panelAddon* create(){
    return new plugInMouvement;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} 
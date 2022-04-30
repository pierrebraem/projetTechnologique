#include "panelAddon.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "myMqtt.hpp"
#include "MySerial.h"
#include <cmath>
#include <string>

class plugInMouvement : public panelAddon{
public:
    //Déclaration de classe "MySerial"
    MySerial *mySerial;

    //Déclaration variable XML
    std::string xmlNomActuel;
    std::string xmlDescription;

    //Déclaration de la classe "myMqtt"
    class myMqtt *mqtt;

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
        xmlNomActuel = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
        xmlDescription = Mouvement_node->first_node("Peripherique")->first_node("Description")->value();

        //Met tous les noms des senseurs dans un tableau afin de les afficher dans des boutons
        int i = 0;
        std::string xmlNoms[5];
        for(Mouvement_node; Mouvement_node; Mouvement_node = Mouvement_node->next_sibling()){
            xmlNoms[i] = Mouvement_node->first_node("Peripherique")->first_node("Nom")->value();
            i++;
        }

        //Met les informations dans l'écran Stone 
        char cmdFormatNom[99];
        strcpy(cmdFormatNom, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"nomdectecteurlabel\",\"text\":\"test\"}>ET");
        //std::cout << cmdFormatNom << "\n";
        //mySerial->writeIt("ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"nomdectecteurlabel\",\"text\":\"test\"}>ET");
        char cmdFormatDescription[99];
        strcpy(cmdFormatDescription, "ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"description\",\"text\":\"test\"}>ET");
        //std::cout << cmdFormatNom << "\n";
        //mySerial->writeIt("ST<{\"cmd_code\":\"set_text\",\"type\":\"label\",\"widget\":\"nomdectecteurlabel\",\"text\":\"test\"}>ET");

        /* std::cout << xmlNomActuel << "\n";
        std::cout << xmlDescription << "\n";
        for(std::string x : xmlNoms){
            std::cout << x << "\n";
        } */

        return 0;
    }

    void DemarrageMqtt(){
        mqtt = new myMqtt("Ecran", "Stone/test", "172.16.206.200", 1883);
    }

    void ArretMqtt(){
        delete mqtt;
    }

    void Publish(const char* message){
        try{
            mqtt->send_msg(message);
        }
        catch(const char* message){
            std::cout << "Une erreur s'est produite lors de l'envoie du message" << "\n";
        }
    }

    bool Subcribe(){
        try{
            bool set = mqtt->receive_msg();
            return set;
        }
        catch(int erreur){
            return 0;
        }
    }
};

extern "C" panelAddon* create(){
    return new plugInMouvement;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} 
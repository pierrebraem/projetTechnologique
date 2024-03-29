/**
    Classe panelAddon : Cette classe est la classe mère de tous les plugins. Elle stock les fonctions nécessaires aux plugins
    @file panelAddon.hpp
    @author Pierre Braem
    @version 1.0 2022-05-01
    
    Historique des versions   
                            Versions  Date        Auteur      Description
                            1.0       2022-05-01  Pierre      Première version de la classe
                            
                            
    librairies utilisées :
    - myMqtt
    - stone
**/

#ifndef PANNEL_ADDON_HPP
#define PANNEL_ADDON_HPP
#include <string>
#include "stone.h"

class panelAddon {
protected:
    double side_length_;

public:
    panelAddon()
        : side_length_(0) {}

    virtual ~panelAddon() {}

    void set_side_length(double side_length) {
        side_length_ = side_length;
    }

    virtual double area() const = 0;

    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp){};
    
    //Fonction pour le capteur de mouvement
    virtual int init(std::string filename, Stone* stone){};
    virtual void lireMouvement(){};
    virtual void ChangerMov(const char* mov, std::string filename, Stone* stone){};

    //Fonction pour le controle de lumière
    virtual void ChangerLamp(const char* lamp, std::string filename, Stone* stone){};
};

// the types of the class factories
typedef panelAddon* create_t();
typedef void destroy_t(panelAddon*);

#endif

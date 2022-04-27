#include "panelAddon.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <cmath>
#include <string>

class plugInMouvement : public panelAddon{
public:
    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }

    /* int nombre1(){
        return 1;
    }

    void initXML(char* Fichierxml){
        rapidxml::file<> xmlFile(Fichierxml);
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
    } */
};

extern "C" panelAddon* create(){
    return new plugInMouvement;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} 
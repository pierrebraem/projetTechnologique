#include "panelAddon.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <cmath>
#include <string>

class plugInMouvement : public panelAddon{
public:
    std::string xmlDate;
    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }

    int nombre1(){
        return 1;
    }

    int init(std::string filename, Stone* stone){
        rapidxml::xml_node<> *root_node;
        rapidxml::file<> xmlFile(filename.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        root_node = doc.first_node("Mouvements");
        rapidxml::xml_node<> *Mouvement_node = root_node->first_node("Mouvement");
        xmlDate = Mouvement_node->first_node("Date")->value();

        std::cout << "coucou" << "\n";

        return 0;
    }
};

extern "C" panelAddon* create(){
    return new plugInMouvement;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} 
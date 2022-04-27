#include "panelAddon.hpp"
#include <cmath>

class plugInMouvement : public panelAddon{
public:
    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }
};

/* extern "C" panelAddon* create(){
    return new plugInMouvement;
}

extern "C" void destroy(panelAddon* p){
    delete p;
} */
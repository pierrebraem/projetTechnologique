#include "panelAddon.hpp"
#include <cmath>

class plugInFumee : public panelAddon {
public:
    virtual double area() const {
        return side_length_ * side_length_ * sqrt(3) / 2;
    }

    virtual int nombre1() const{
        return 1
    }
};


// the class factories
extern "C" panelAddon* create() {
    return new plugInFumee;
}


extern "C" void destroy(panelAddon* p) {
    delete p;
}

/* extern "C" int nombre1(){
    return 1;
} */
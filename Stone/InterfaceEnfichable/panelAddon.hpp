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
    virtual int init(std::string filename, Stone* stone);
};

// the types of the class factories
typedef panelAddon* create_t();
typedef void destroy_t(panelAddon*);

#endif

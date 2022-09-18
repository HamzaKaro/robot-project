#include "global.hpp"

class LedMatrix
{
private:
    int size_;
    Port port_;
    int startPin_;
    //Led* leds_;
    //void setPorts(uint8_t);

public:
    // Pins are only connected to 1 port, and all pins must be next to
    //  each other. size4 must start at 0, size3 must start 0-2, etc.
    void update(uint8_t param);

    // Pins are only connected to 1 port, and all pins must be next to
    //  each other. size4 must start at 0, size3 must start 0-2, etc.
    LedMatrix(uint16_t size, Port port, uint16_t startPin);
};

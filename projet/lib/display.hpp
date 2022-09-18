#include "global.hpp"
#include "pin.hpp"

class Display
{
private:
    Pins valuePins_;
    Pin enablePin_;
    Pin resetPin_;

public:
    const uint8_t pinCount = 4;

    Display(Port, uint8_t,
            Pin, Pin);

    void set(uint8_t);
    void reset();
};


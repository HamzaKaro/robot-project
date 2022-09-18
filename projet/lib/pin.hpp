#pragma once
#include "global.hpp"

class Pins
{
public:
    Port startPort_;          // Either A, B, C or D (the port of the pins)
    uint8_t startPin_;        // From 0 to 7 (the pin of least significative bit)
    uint8_t size_;
    uint8_t maxValue_;

public:
    Pins() = default;
    Pins(Port startPort, uint8_t startPin, uint8_t size);

    uint8_t get();
    void set(uint8_t);
};



// Pin is a Pins object with  size = 1
class Pin : public Pins
{
public:
    Pin() = default;
    Pin(Port port, uint8_t pin);
};





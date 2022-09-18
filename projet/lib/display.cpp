#include "display.hpp"

Display::Display(Port startPort, uint8_t startPin,
                 Pin enablePin, Pin resetPin)
{
    valuePins_ = Pins(startPort, startPin, pinCount);
    enablePin_ = enablePin;
    resetPin_ = resetPin;
    reset();
}

void Display::set(uint8_t value)
{
    resetPin_.set(0);
    valuePins_.set(value);
    // Set the enable port to set the value
    enablePin_.set(1);
    enablePin_.set(0);
}

void Display::reset()
{
    resetPin_.set(1);
}


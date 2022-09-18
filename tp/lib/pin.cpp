#include "pin.hpp"

Pin::Pin(Port port, uint8_t pin) : Pins(port, pin, 1) { }
Pins::Pins(Port startPort, uint8_t startPin, uint8_t size) :
    startPort_(startPort), startPin_(startPin), size_(size)
{
    maxValue_ = (1 << size_) - 1;   // Max value for this number of pins,
                                    //  no matter the position
}

uint8_t Pins::get()
{
    switch (startPort_)
    {
        case A:
            return maxValue_ & (PINA >> startPin_);
        case B:
            return maxValue_ & (PINB >> startPin_);
        case C:
            return maxValue_ & (PINC >> startPin_);
        case D:
            return maxValue_ & (PIND >> startPin_);
    }

    return 0;
}

void Pins::set(uint8_t value)
{
    if (value > maxValue_)
        value = maxValue_;

    switch (startPort_)
    {
        case A:
            PORTA = (PORTA & ~(maxValue_ << startPin_)) | (value << startPin_);
            break;
        case B:
            PORTB = (PORTB & ~(maxValue_ << startPin_)) | (value << startPin_);
            break;
        case C:
            PORTC = (PORTC & ~(maxValue_ << startPin_)) | (value << startPin_);
            break;
        case D:
            PORTD = (PORTD & ~(maxValue_ << startPin_)) | (value << startPin_);
            break;
    }
}

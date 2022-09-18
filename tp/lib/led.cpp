
#include "led.hpp"

void SetPort(Port port, uint16_t pin, uint16_t value)
{
    switch (port)
    {
        case A:
            PORTA = (((1 << pin) ^ 0xFF) & PORTA) | (value << pin);
            break;
        case B:
            PORTB = (((1 << pin) ^ 0xFF) & PORTB) | (value << pin);
            break;
        case C:
            PORTC = (((1 << pin) ^ 0xFF) & PORTC) | (value << pin);
            break;
        case D:
            PORTD = (((1 << pin) ^ 0xFF) & PORTD) | (value << pin);
            break;
    }
};

Led::Led(Port port1, uint8_t pin1, Port port2, uint8_t pin2)
{
    colour_ = SHUT;
    lastColourAsGold_ = GREEN;

    port1_ = port1;
    pin1_ = pin1;
    port2_ = port2;
    pin2_ = pin2;
}

void Led::setLedPorts(uint8_t new1, uint8_t new2)
{
    SetPort(port1_, pin1_, new1);
    SetPort(port2_, pin2_, new2);
}

void Led::changeColour(Colour newColour)
{
    colour_ = newColour;
}

void Led::update()
{
    switch (colour_)
    {
        case SHUT: setLedPorts(0, 0); break;

        case GREEN: setLedPorts(1, 0); break;

        case RED: setLedPorts(0, 1); break;

        case GOLD:
            switch (lastColourAsGold_)
            {
                case SHUT: break;

                case GOLD: break;

                case GREEN:
                    lastColourAsGold_ = RED;
                    setLedPorts(0, 1);
                    break;

                case RED:
                    lastColourAsGold_ = GREEN;
                    setLedPorts(1, 0);
                    break;
            }
            break;
    }
}

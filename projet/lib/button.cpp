
#include "button.hpp"



Button::Button(Pin pin)
{
    pin_ = pin;
}



void Button::update()
{
    switch (state_)
    {
        case UNPUSHED:
            // Debounce
            if (debounce())
                state_ = HIGH_EDGE;
            break;

        case HIGH_EDGE:
            // Pulse should only last 1 cycle
            state_ = PENDING;
            
        case PENDING:
            // Reset on release
            if (!(debounce()))
                state_ = LOW_EDGE;
            break;

        case LOW_EDGE:
            // Pulse should only last 1 cycle
            state_ = UNPUSHED;

        default:
            // Nothing special here, code shouldn't reach this part
            break;
    }
}



bool Button::debounce()
{
    const double DELAY_DEBOUNCE = 10;   // Time (in ms) to wait for the debounce
    if (pin_.get())
    {
        _delay_ms(DELAY_DEBOUNCE);
        return pin_.get();
    }
    else
        return false;
}

Pin& Button::getPin() { return pin_; }

bool Button::getPressed() { return debounce(); }
bool Button::getHighEdge() { return (state_ == HIGH_EDGE); }
bool Button::getLowEdge() { return (state_ == LOW_EDGE); }


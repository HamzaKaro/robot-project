/*
 * Name : Hamza Karoui (2012635),David Amoussa (2085243),Alexandre Turcotte (2087684), Huber Boucher
 * Travail : TRAVAIL_PRATIQUE 2
 * Section # : 5
 * team # : 211212
 * corrector : Eya-Tom Augustin Sangam, Tristan Rioux 
 * Description: this code is used to turn on Led with any colour of our choice.
 * we can also put the LED in any PIN of our choice
 */

#include "global.hpp"

class Led
{
private:
    Colour colour_;
    Colour lastColourAsGold_;
    Port port1_; Port port2_;
    uint8_t pin1_; uint8_t pin2_;

    void setLedPorts(uint8_t new1, uint8_t new2);

public:
    // Initializes the object with the SHUT state
    Led(Port, uint8_t, Port, uint8_t);
    void update();
    // According to the enum: Colour {GREEN, RED, GOLD, SHUT}
    void changeColour(Colour);
};


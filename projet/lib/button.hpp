/*
 * Name : Hamza Karoui (2012635),David Amoussa (2085243),Alexandre Turcotte (2087684), Huber Boucher
 * Travail : TRAVAIL_PRATIQUE 2
 * Section # : 5
 * team # : 211212
 * corrector : Eya-Tom Augustin Sangam, Tristan Rioux 
 * Description: this code is used to add buttons easily to our main function.
 * this class verifies the state of the button and apply debounce function so that we avoid 
 * any false pulse.
 */

#include "global.hpp"

#include "pin.hpp"



class Button
{
private:

    enum PushState                      // Handle pulse generation
    {
        UNPUSHED,   // Not pressed
        HIGH_EDGE,  // Pressed, for one and only one cycle
        LOW_EDGE,   // Unpressed, for one and only one cycle
        PENDING     // Maintained after the first cycle
    };

    Pin pin_;       // The pin the button is connected to
    PushState state_;    // The push state of the button to handle pulse generation

    // Return true if the button is pressed, with debounce
    bool debounce();

public:
    // Return pin
    Pin& getPin();

    // Return true as long as the button is pressed
    bool getPressed();
    // Return true on the first cycle the button is pressed/unpressed only
    bool getHighEdge();
    bool getLowEdge();

    // MUST be called every cycle to update state
    void update();
    Button(Pin pin);
};
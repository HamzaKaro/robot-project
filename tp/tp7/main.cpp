#include <avr/io.h>

 #include "button.hpp"
 #include "can.hpp"
 #include "led.hpp"
 #include "pwm.hpp"
 #include "memoryAccess.hpp"
/**
 * permet de tester toutes les classe de lib211212.a
 * */
int main()
{
    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0x00;
    DDRD = 0xFF;



    bool TEST_EEPROM = 0;
    

    /* can canTest; */

    if (TEST_EEPROM)
    {
        // Set the EEPROM
        Memory memory(0);
        Led memoryLed(A, 0, A, 1); // Green if the communication is ok, red otherwise

        // Test the EEPROM
        unsigned char test[5]  = "allo";
        memory.writeEeprom(test, 5);
        unsigned char verif[5];
        memory.readEeprom(*verif, 5);

        if(memory.isChar1EqualChar2(test, verif))
            memoryLed.changeColour(GREEN);
        else
            memoryLed.changeColour(RED);
        memoryLed.update();
    }

    /* TEST_BUTTON */
    // Set the state enum
    enum State {A1, A2, A3, A4};
    State state = A1;
    // Set the buttons and LED
    Button buttonC7(C, 3);
    Button buttonC6(C, 2);
    Led led(B, 0, B, 1);

    /* TEST_PWM *
    // Set the PWM
    uint8_t cycle = 0;
    uint8_t speed = 0;
    Pwm pwm(speed, speed);*/

    for (;;)
    {
        // Update LED and buttons
        led.update();
        buttonC7.update();
        buttonC6.update();

        // Next state when C7 pressed ; reset when C6 pressed
        if (buttonC7.getPulse())
        {
            switch (state)
            {
                case A1: state = A2; break;
                case A2: state = A3; break;
                case A3: state = A4; break;
                case A4: break;
            }
        }
        if (buttonC6.getPulse())
            state = A1;

        // Set colour according to state
        switch (state)
        {
            case A1: led.changeColour(SHUT); break;
            case A2: led.changeColour(GREEN); break;
            case A3: led.changeColour(RED); break;
            case A4: led.changeColour(GOLD); break;
        }
        
        /*
        // Every 50 cycles, increase speed by 10%, then reset
        cycle++;
        if (cycle > 10)
        {
            cycle = 0;
            speed += 10;
            if (speed > 100)
                speed = 0;
            pwm.setIntensity(speed, speed);
        }
        */
    }

    return 0;
}
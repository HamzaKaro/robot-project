#include <avr/io.h>

 #include "button.hpp"
 #include "can.hpp"
 #include "led.hpp"
 #include "pwm.hpp"
 #include "memoryAccess.hpp"
 #include "debug.hpp"

/**
 * permet de tester toutes les classe de lib211212.a
 * */
int main()
{
    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0x00;
    DDRD = 0xFF;

    DEBUG_PRINT((-1234));
    DEBUG_PRINT((8403));

    return 0;
}
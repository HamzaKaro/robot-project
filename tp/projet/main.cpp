#include "interface.hpp"

using namespace UART;

volatile uint8_t row;
volatile uint8_t col;
volatile uint8_t result;
volatile uint8_t count;
volatile bool dontRepeat = true;

//Inturrupt on C
ISR(PCINT2_vect)
{
    cli();
    row = col;
    col = PINC & 0xFC;
    switch (count%3)
    {
    case 0:
        DDRC = 0x1F;
        Pins(C, 2, 6).set(0x07); // 6-bit
        PCMSK2 = (1 << PCINT23) | (1 << PCINT22) | (1 << PCINT21);
        break;

    case 1:
        DDRC = 0xE0;
        Pins(C, 2, 6).set(0x38); // 6-bit
        PCMSK2 = (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20);
        break;
    
    case 2:
        if(dontRepeat){
            result = (row & col) >> 2;
            Interface::changeConfig(result);
            _delay_ms(100);
        }
        dontRepeat = !(dontRepeat);
        break;
    }
    count += 1;
    sei();
}



ISR(TIMER0_OVF_vect)
{
    cli();
    Interface::addTime(0.8192);
    sei();
}



void initialize(){

    cli();

    PCICR |= (1 << PCIE2); //To autorise the interupt on PORTC
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20);//0xE0;   // Pin Change Mask Register 2 to enable PCINT23:21

    TCCR0A = (1 << WGM11) | (1 << WGM10);
    TCCR0B = (1 << CS02) | (0 << CS00) | (1 << WGM13);
    TIMSK0 = (1 << TOIE0);
    TCNT0 = 0x00;

    sei();
}

int main()
{
    initialize();
    Interface::robot.startup();
    
    return 0;
}
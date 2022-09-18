#include "matrix.hpp"

void LedMatrix::update(uint8_t param){

    //Max value of the param according to the size_
    uint8_t limit = (1 << (size_ *2))- 1;

    //We don't want a param to big for the size of the matrix
    if(param > limit){
        param = limit;
    }
       
    switch (port_)
    {
        case A:
            PORTA = (PORTA & ~(limit << startPin_)) | (param << startPin_);
            break;
        case B:
            PORTB = (PORTB & ~(limit << startPin_)) | (param << startPin_);
            break;
        case C:
            PORTC = (PORTC & ~(limit << startPin_)) | (param << startPin_);
            break;
        case D:
            PORTD = (PORTD & ~(limit << startPin_)) | (param << startPin_);
            break;
    }
}

LedMatrix::LedMatrix(uint16_t size, Port port, uint16_t startPin)
{
    size_ = size;
    port_ = port;

    switch (size)
    {
        case 2:
            if (startPin > 4)
                startPin = 4;
            break;
        case 3:
            if (startPin > 2)
                startPin = 2;
            break;
        case 4:
            if (startPin > 0)
                startPin = 0;
            break;
    }
    startPin_ = startPin;
    update(0);
}

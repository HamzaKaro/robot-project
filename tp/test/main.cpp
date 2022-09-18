
#include <avr/io.h>
#include "uart.hpp"
#include "sensor.hpp"
using namespace UART;

void printOrder(uint16_t instruc){
    dPrint(instruc);
    dPrint("\n-----------------");
}


int main()
{
    Sensor cap(Sensor::E_CAN);
    initialisationUART(0xcf,1);
    for (;;){
        printOrder(cap.getDistance()/10);// afficher real
    }

    return 0;
}
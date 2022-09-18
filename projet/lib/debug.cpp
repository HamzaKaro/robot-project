#include "debug.hpp"

using namespace UART;

void initUART(){
    initialisationUART(0xCF, 1);
}

void debugPrint(uint16_t x){
    dPrint(x);
}

void debugPrint(const char* x){
    dPrint(x);
}

void debugPrint(unsigned char* x){
    dPrint(x);
}


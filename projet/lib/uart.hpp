#include "global.hpp"

namespace UART{

    /*
    Function to init te UART transmission with a size of 8 bits and 1 stop bit.
    Setting:
        1- The baud setting you want (51 for tp9)
        2- The option for transmit or receive UART (1=both, 2=receive only, 3=transmit only)
    */
    void initialisationUART (uint16_t, uint16_t );

    /*
    Function to read one byte received by UART transmission
    */
    unsigned char USART_Receive();

    /*
    Function to print a const char in Serial Monitor  (direct declared char[])
    */
    void dPrint(const char charList[]);

    /*
    Function to print an unsigned char in Serial Monitor  (variable wich is unsigned char)
    */
    void dPrint(unsigned char charList[]);

    /*
    Function to print an int in Serial Monitor 
    */
    void dPrint(uint16_t number);


    void lineBreak();
    
}

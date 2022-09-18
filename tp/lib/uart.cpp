#include "uart.hpp"

namespace Private{

    void transmitUART (uint8_t data)
    {
        while ( !( UCSR0A & (1 << UDRE0)));
        UDR0 = data;
    };

    void lPrint(uint16_t number, bool isLast)
    {
        if (number < 0)
        {
            transmitUART('-');
            number = -number;
        }

        if (number < 10)
        {
            transmitUART(number + '0');
        }
        else
        {
            uint16_t unit = number % 10;
            number = (number - unit) / 10;
            lPrint(number, false);
            transmitUART(unit + '0');
        }
    };
}

void UART::initialisationUART (uint16_t baud = 0xCF, uint16_t option = 1)
{
    UBRR0H = (baud & 0xF00) >> 8;
    UBRR0L = (baud & 0x0FF);

    UCSR0A = (1 << UDRE0);
    
    option &= 0x0003; //To assure the value between 1 and 3
    switch (option)
    {
    case 0:
        //Please select a option
        break;
    case 1:
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);
        break;
    case 2:
        UCSR0B = (1 << RXEN0);
        break;
    case 3:
        UCSR0B = (1 << TXEN0);
        break;
    }  

    UCSR0C = (0 << USBS0) | (3 << UCSZ00);
}

unsigned char UART::USART_Receive()
{
    while ( !(UCSR0A & (1<<RXC0)) ){_delay_ms(10);}
    return UDR0;
}

void UART::dPrint(uint16_t number)
{
    Private::lPrint(number, true);
}

void UART::dPrint(unsigned char charList[])
{
    // inspired by https://stackoverflow.com/questions/21022644/how-to-get-the-real-and-total-length-of-char-char-array/60009353#60009353
    for (int i = 0;; i++)
    {
        if (charList[i] == '\0')
            break;
        Private::transmitUART(charList[i]);
    }
}

void UART::dPrint(const char charList[])
{
    // inspired by https://stackoverflow.com/questions/21022644/how-to-get-the-real-and-total-length-of-char-char-array/60009353#60009353
    for (int i = 0;; i++)
    {
        if (charList[i] == '\0')
            break;
        Private::transmitUART(charList[i]);
    }
}

void UART::lineBreak()
{
    Private::transmitUART('\n');
}



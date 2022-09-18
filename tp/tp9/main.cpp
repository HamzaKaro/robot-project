#include <avr/io.h>

#include "led.hpp"
#include "button.hpp"
#include "can.hpp"
#include "matrix.hpp"
#include "motor.hpp"
#include "memoryAccess.hpp"
#include "debug.hpp"
#include "uart.hpp"

using namespace UART;

void printOrder(uint8_t instruc, uint8_t operat){
    dPrint(instruc);
    dPrint(operat);
    dPrint("\n-----------------");
}

void updateMotor(Motor& motor, LedMatrix& matrix){
switch (motor.getOrientation())
{
    case 0: matrix.update(0b100101); break;
  
    case 1: matrix.update(0b010011); break;

    case 2: matrix.update(0b001101); break;

    case 3: matrix.update(0b010110); break;
  
default:
    break;
}
}

int main()
{

    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0x30;

    initialisationUART(0x33, 0x1);

    Memory memory(0);
    unsigned char valeur;

    for (;;)
    {
        valeur = USART_Receive();
        memory.writeEeprom(&valeur, 1);
        if (valeur == 0xFF)
            break;
    }

    memory.setAddress(0);

    uint8_t loopAddress=0;
    uint8_t nLoop=0;

    unsigned char instruction;
    unsigned char op;
    Led biColorLed(A, 0, A, 1);
    LedMatrix matrix4 = LedMatrix(4, C, 0);
    LedMatrix matrix3 = LedMatrix(3, A, 2);

    Motor motor(D, 3);
    bool exit = false;
    bool start = false;

    while(!exit){
        memory.readEeprom(instruction, 1);
        memory.readEeprom(op, 1);
        PORTB = instruction;
        printOrder(instruction, op);
        uint8_t percent = op*(100.0/255.0);
        if(instruction == 0x64){
            dPrint(op);
        }
        if(instruction != 0x01 && not start)
            continue;
        
        switch (instruction){
            case 0x01:  //Start
                biColorLed.changeColour(GOLD);
                for(int i=0; i <100; i++){
                    _delay_ms(5);
                    biColorLed.update();
                }
                start = true;
                break;

            case 0x02:  //Wait
                for(int i=0; i<op; i++)
                    _delay_ms(25);
                break;

            case 0x044: //Turn on LED
                if(op<128){
                    biColorLed.changeColour(GREEN);
                }
                else{
                    biColorLed.changeColour(RED);
                }
                break;
            
            case 0x045: //Turn off LED
                biColorLed.changeColour(SHUT);
                break;
            
            case 0x18:  //Turn on LED matrix
                matrix4.update(op);
                break;

            case 0x19:  //Turn off LED matrix
                matrix4.update(0);
                break;
            
            case 0x60:  //Stop the engine
            case 0x61:
                DEBUG_PRINT("MAR");
                motor.setIntensity(0,0);
                break;
            
            case 0x62:  //Go forward
                DEBUG_PRINT("MAV");
                if(motor.getIsBackward() != 0) 
                    motor.rotate(2);
                motor.setDirection(true);
                updateMotor(motor, matrix3);
                motor.setIntensity(percent, percent);
                _delay_ms(1000);
                break;
            
            case 0x63:  //Go backward
                DEBUG_PRINT("MRE");
                if(motor.getIsBackward() == 0) 
                    motor.rotate(2);
                motor.setDirection(false);
                updateMotor(motor, matrix3);
                motor.setIntensity(percent, percent);
                _delay_ms(1000);
                break;

            case 0x64:  //Turn right
                if(motor.getIsBackward() == 0) 
                    motor.rotate(1);
                else
                    motor.rotate(3);
                updateMotor(motor, matrix3);
                motor.setIntensity(100, 0);
                _delay_ms(1000);
                motor.setIntensity(0,0);
                break;

            case 0x65:  //Turn left
                if(motor.getIsBackward() == 0) 
                    motor.rotate(3);
                else
                    motor.rotate(1);
                updateMotor(motor, matrix3);
                motor.setIntensity(0,100);
                _delay_ms(1000);
                motor.setIntensity(0,0);
                break;
            
            case 0xC0:  //Start loop
                loopAddress=memory.getAddress();
                nLoop=op;
                break;
            
            case 0xC1:  //End loop
                if (nLoop>0){
                    nLoop--;
                    memory.setAddress(loopAddress);
                }
                break;

            case 0xFF:  //End
                biColorLed.changeColour(RED);
                exit = true;
                break;
        }
    
    biColorLed.update();
    }

    return 0;
}
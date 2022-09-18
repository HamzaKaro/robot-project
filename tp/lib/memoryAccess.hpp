/*
 * Name : Hamza Karoui (2012635),David Amoussa (2085243),Alexandre Turcotte (2087684), Huber Boucher
 * Travail : TRAVAIL_PRATIQUE 2
 * Section # : 5
 * team # : 211212
 * corrector : Eya-Tom Augustin Sangam, Tristan Rioux 
 * Description: This programm can be use to write and read in the EEPROM memory of
 * *            a microcontroller AtMega16 of Atmel.
 */

#include "global.hpp"

using namespace std;

//Class Memory: usefull for writing and reading in the EEPROM memory of the microcontroller
class Memory{
private:
    //Current memory address in bytes
    uint16_t address_;

public:
    
    Memory();
    Memory(uint16_t);
    void setAddress(uint16_t);
    void moveAddress(uint16_t);
    uint16_t getAddress();
    void writeEeprom(unsigned char*, uint8_t);
    void readEeprom(unsigned char&,uint8_t);
    bool isChar1EqualChar2(unsigned char*, unsigned char*);
};
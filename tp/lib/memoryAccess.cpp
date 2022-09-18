#include "memoryAccess.hpp"

using namespace std;


//Default constructor with adress set to 0
Memory::Memory(){
    address_ = 0;
}  

//Constructor where you set the class setting
Memory::Memory(uint16_t memoryAdress){
    address_ = memoryAdress;
}  

//Set the adress to write in memory
void Memory::setAddress(uint16_t memoryAddress){
    address_ = memoryAddress;
}

//Set the value of address_ with (address_ + uint16_t)
void Memory::moveAddress(uint16_t number){
    address_ += number;
}

//If you want to know where you are in the memory
uint16_t Memory::getAddress(){
    return address_;
}

//Function to write a char buffer in EEPROM
//Setting:
//      1- Table of charater to write
//      2- Number of bytes of the buffer you want to write
void Memory::writeEeprom(unsigned char message[], uint8_t size){
  
    eeprom_write_block((void*)message, (void*)address_, size);
    address_ += size;
}

//Function to read in EEPROM according address value define by (address_ - size)
//Setting:
//      1-Char buffer to read pass by reference
//      2-Number of bytes of the buffer you want to read
void Memory::readEeprom(unsigned char &buffer, uint8_t size){

    eeprom_read_block((void*)&buffer, (const void*)address_, size);
    address_ += size;
}

//Function to check if the firs char is equal to the second char.
//Setting:
//      1- First char buffer
//      2- Second char buffer
//Return False if sizeof(buffer1) != sizeof(buffer2) or if a char in the buffer1 != of a other
//char in the buffer2 at the same position.
bool Memory::isChar1EqualChar2(unsigned char* buffer1, unsigned char* buffer2){
    if(sizeof(buffer1) == sizeof(buffer2)){
        for(unsigned int i=0; i < (sizeof(*buffer1)/sizeof(unsigned char)); i++){
            if (buffer1[i] != buffer2[i]) return false;
        }
    }
    else return false;

    return true;
}

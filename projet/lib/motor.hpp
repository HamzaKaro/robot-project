/*
 * Name : Hamza Karoui (2012635),David Amoussa (2085243),Alexandre Turcotte (2087684), Huber Boucher
 * Travail : TRAVAIL_PRATIQUE 2
 * Section # : 5
 * team # : 211212
 * corrector : Eya-Tom Augustin Sangam, Tristan Rioux 
 * Description: this program generates Pwm correct using Timer1
 * and compare the signal with OACR1A and OACR1B registers
 */

#include "global.hpp"
#include "pin.hpp"

/**
 * Classe Pwm:
 *   generates pwm correct signal
 **/
class Motor
{
public:
/**
 *   constructor: init variables
 **/
   Motor(Pin , Pin);

/**
 *   setIntensity: convert pourcentA and pourcentB to values between 0 and 255
 *   so that we can use them to configure registers
 * 
 *   parameters : pourcentA:uint8_t pourcentB:uint8_t 
 *   return: void
 **/ 
   void setIntensity(int8_t pourcentA,int8_t pourcentB);

/**
 *   setIntensity: convert pourcentA and pourcentB to values between 0 and 255
 *   so that we can use them to configure registers
 * 
 *   parameters : Port port, uint8_t pin, bool isGoingBackward 
 *   return: void
 **/ 
   

   
private:
   Pin dirPin1_;
   Pin dirPin2_;
};



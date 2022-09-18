
#include "sensor.hpp"
#include "can.hpp"


// constructeur: initialisation du convertisseur
Sensor::Sensor()
{
   typeCan_=I_CAN;
   DDRB=0b10;
   DDRA=0b1101;
}
Sensor::Sensor(TypeCan typeCan)
{  
   typeCan_=typeCan;
   DDRB=0b11101;
   DDRA=0b1101;
   
}
/**
 * formule distance L : 1360.743/(L+0.42)+4.316=valeurCan_ => L=1360.743/(valeurCan_-4.316)-0.42
 * 
 **/
float Sensor::getDistance(){
   uint16_t nBits;
   if (typeCan_==I_CAN){
      can convert;
      valeurCan_=convert.lecture(I_CAN);
      nBits=1025;
   }
   else{
      PORTA=0b01;
      setValeurEx();
      nBits=255;
   }
   if ((valeurCan_/1024.0*5.0)<0.54)
      distance_=263.6755*nBits/5.0/((valeurCan_)-0.1017*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
   else if ((valeurCan_/1024.0*5.0)<0.62)
      distance_=239.7104*nBits/5.0/((valeurCan_)-0.1421*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
   else if ((valeurCan_/1024.0*5.0)<0.68)
      distance_=270.4790*nBits/5.0/((valeurCan_)-0.0795*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
   else if ((valeurCan_/1024.0*5.0)<0.86)
      distance_=283.8138*nBits/5.0/((valeurCan_)-0.0501*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
   else if ((valeurCan_/1024.0*5.0)<1.39)
      distance_=247.8138*nBits/5.0/((valeurCan_)-0.1538*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
   else
      distance_=227.4258*nBits/5.0/((valeurCan_)-0.2553*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
   return distance_;
}
uint8_t Sensor::getValCan(){
   return valeurCan_;
}
void Sensor::changeCapteur(Position position){
   PORTA=(PORTA & (~(0b111<<B1)))|(position<<numPinControle);
}
void Sensor::setTypeCan(TypeCan typeCan){
   typeCan_=typeCan;
}
void Sensor::setValeurEx(){
   valeurCan_=0;
   for (uint8_t i=0 ; i<8;i++){
      PORTB=(PORTB &(~(0b111<<numPinControle)))|(i<<numPinControle); //set B2, B3 et B4
      _delay_ms(10);
      valeurCan_|=((PINB & 2)>>1)<<(7-i);
   }
};
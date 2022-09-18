#include "sensor.hpp"
#include "can.hpp"
// constructeur: initialisation du convertisseur
Sensor::Sensor()
{
   typeCan_ = I_CAN;
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
bool Sensor::updateDistance()
{
   bool isChanged = false;
   uint16_t nBits=1025;
   can converter;
   Sensor::Position positions[3] = {LEFT, CENTER, RIGHT};
   for (Sensor::Position pos : positions)
   {
      changeSensor(pos);
      switch (typeCan_)
      {
         case I_CAN:
            valeurCan_ = (converter.lecture(I_CAN) );
            nBits=1025;
            break;
         
         case E_CAN:
            PORTA = PORTA | 0x1;
            setValeurEx();
            nBits=255;
            break;
      }
      // Appliquer des formules d'ajustements sur les points trouver dans le document de reference
      //    les formules sont des estimés lineaires de la distance en fonction des inputs
      uint8_t newDist;
      if ((valeurCan_/1024.0*5.0)<0.54)
         newDist=263.6755*nBits/5.0/((valeurCan_)-0.1017*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/1024.0*5.0)<0.62)
         newDist=239.7104*nBits/5.0/((valeurCan_)-0.1421*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/1024.0*5.0)<0.68)
         newDist=270.4790*nBits/5.0/((valeurCan_)-0.0795*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/1024.0*5.0)<0.86)
         newDist=283.8138*nBits/5.0/((valeurCan_)-0.0501*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/1024.0*5.0)<1.39)
         newDist=247.8138*nBits/5.0/((valeurCan_)-0.1538*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else
         newDist=227.4258*nBits/5.0/((valeurCan_)-0.2553*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      
      if (newDist != distances_[pos]*10)
         isChanged = true;
      distances_[pos] = newDist/10.0;
   }
   return isChanged;
}
bool Sensor::updateStatus()
{
   bool isChanged = false;
   Sensor::Position positions[3] = {LEFT, CENTER, RIGHT};
   for (Sensor::Position pos : positions)
   {
      Status newStatus;
      if (distances_[pos] < BOUND_UNDER_OK)
         newStatus = NONE;
      else if (distances_[pos] < BOUND_OK_WARNING)
         newStatus = OK;
      else if (distances_[pos] < BOUND_WARNING_DANGER)
         newStatus = WARNING;
      else
         newStatus = DANGER;
      if (newStatus != status_[pos])
         isChanged = true;
      status_[pos] = newStatus;
   }
   return isChanged;
}
uint8_t Sensor::getValCan()
{
   return valeurCan_;
}
void Sensor::changeSensor(Position position)
{
   PORTA = (PORTA & (~(0x3 << numPinControle))) | (position << numPinControle);
}
void Sensor::setTypeCan(TypeCan typeCan)
{
   typeCan_ = typeCan;
}
void Sensor::setValeurEx()
{
   valeurCan_ = 0;
   for (uint8_t i = 0; i < 8; i++)
   {
      PORTB = (PORTB &(~(0x7 << numPinControle))) | (i << numPinControle); //set B2, B3 et B4
      _delay_ms(10);
      valeurCan_ |= ((PINB & 0x2) >> 1) << (7 - i);
   }
};

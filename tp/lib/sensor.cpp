#include "sensor.hpp"

// constructeur: initialisation du convertisseur
Sensor::Sensor(
   TypeCan typeCan, Pin pinType,
   Pins pinsControleInterne,
   Pins pinsControleExterne
)
{  
   typeCan_ = typeCan;
   pinType_ = pinType;
   pinsControleInterne_ = pinsControleInterne;
   pinsControleExterne_ = pinsControleExterne;
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

   // KNOWN BUG :    First position's data overwrites second iteration
   // RESOLVE :      Duplicate first iteration over second
   Sensor::Position positions[4] = {LEFT, LEFT, CENTER, RIGHT};
   
   for (Sensor::Position pos : positions)
   {
      changeSensor(pos);
      switch (typeCan_)
      {
         case I_CAN:
            pinType_.set(0);
            valeurCan_ = (converter.lecture());
            nBits=1025;
            break;
         
         case E_CAN:
            pinType_.set(1);
            setValeurEx();
            nBits=255;
            break;
      }
      // Appliquer des formules d'ajustements sur les points trouver dans le document de reference
      //    les formules sont des estimés lineaires de la distance en fonction des inputs
      uint16_t newDist;
      if ((valeurCan_/nBits*5.0)<0.54)
         newDist=263.6755*nBits/5.0/((valeurCan_)-0.1017*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/nBits*5.0)<0.62)
         newDist=239.7104*nBits/5.0/((valeurCan_)-0.1421*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/nBits*5.0)<0.68)
         newDist=270.4790*nBits/5.0/((valeurCan_)-0.0795*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/nBits*5.0)<0.86)
         newDist=283.8138*nBits/5.0/((valeurCan_)-0.0501*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else if ((valeurCan_/nBits*5.0)<1.39)
         newDist=247.8138*nBits/5.0/((valeurCan_)-0.1538*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      else
         newDist=227.4258*nBits/5.0/((valeurCan_)-0.2553*nBits/5.0)-0.42;//appliquer la formule trouver dans le document de reference
      
      if (newDist != distances_[pos])
         isChanged = true;
      distances_[pos] = newDist;
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
      if (distances_[pos] >= BOUND_MIN_OK)
         newStatus = OK;
      else if (distances_[pos] >= BOUND_MIN_WARNING)
         newStatus = WARNING;
      else if (distances_[pos] >= BOUND_MIN_DANGER)
         newStatus = DANGER;
      else
         newStatus = NONE;
      if (newStatus != status_[pos])
         isChanged = true;
      status_[pos] = newStatus;
   }
   return isChanged;
}
uint16_t Sensor::getValCan()
{
   return valeurCan_;
}
void Sensor::changeSensor(Position position)
{
   pinsControleInterne_.set(position);
}
void Sensor::setTypeCan(TypeCan typeCan)
{
   typeCan_ = typeCan;
}
void Sensor::setValeurEx()
{
   valeurCan_ = 0;
   for (uint16_t i = 0; i < 8; i++)
   {
      pinsControleExterne_.set(i);
      _delay_ms(10);
      valeurCan_ |= ((PINB & 0x2) >> 1) << (7 - i);
   }
};

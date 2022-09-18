#include "global.hpp"

#include "pin.hpp"
#include "can.hpp"
#include "uart.hpp"

/*
 * Classe can:
 *   Le constructeur initialise le convertisseur.
 *   Une lecture enclanche une conversion et le resultat
 *   est retourne sur 16 bits.
 *
 */
class Sensor
{
public:
   enum Position  { LEFT = 0x0, CENTER = 0x1, RIGHT = 0x2 };
   enum Status  { NONE, OK, WARNING, DANGER };
   enum TypeCan  { E_CAN, I_CAN };
   const uint16_t BOUND_MIN_DANGER = 100;
   const uint16_t BOUND_MIN_WARNING = 200;
   const uint16_t BOUND_MIN_OK = 500;
   Sensor(
      TypeCan typeCan, Pin pinType,
      Pins pinsControleInterne,
      Pins pinsControleExterne
   );
   uint16_t getValCan();
   void setTypeCan(TypeCan typeCan);
   bool updateDistance();
   bool updateStatus();
   uint16_t getDistLeft() { return distances_[LEFT]; }
   uint16_t getDistCenter() { return distances_[CENTER]; }
   uint16_t getDistRight() { return distances_[RIGHT]; }
   Status getStatusLeft() { return status_[LEFT]; }
   Status getStatusCenter() { return status_[CENTER]; }
   Status getStatusRight() { return status_[RIGHT]; }
   
private:
   void changeSensor(Position position);
   void setValeurEx();

   uint16_t distances_[3] = {0, 0, 0};
   Status status_[3] = {OK, OK, OK};
   uint16_t valeurCan_=0;

   TypeCan typeCan_;
   Pin pinType_;
   Pins pinsControleInterne_;
   Pins pinsControleExterne_;
};


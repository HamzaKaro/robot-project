#include "global.hpp"
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
   const uint8_t numPinControle=2;
   const uint8_t BOUND_UNDER_OK = 10;
   const uint8_t BOUND_OK_WARNING = 20;
   const uint8_t BOUND_WARNING_DANGER = 50;
   Sensor();
   Sensor(TypeCan);
   uint8_t getValCan();
   void setTypeCan(TypeCan typeCan);
   bool updateDistance();
   bool updateStatus();
   
private:
   void changeSensor(Position position);
   void setValeurEx();
   uint8_t distances_[3] = {0, 0, 0};
   Status status_[3] = {OK, OK, OK};
   uint8_t valeurCan_=0;
   TypeCan typeCan_;
};


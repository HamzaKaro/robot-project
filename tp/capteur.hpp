/*
 * Classe permettant l'acces au convertisseur analogique/numerique
 * du microcontroleur ATMega16 de Atmel.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1995
 *
 * Matthew Khouzam et Jerome Collin
 * 2005-2006
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef CAN_H
#define CAN_H

#include "global.hpp"

/*
 * Classe can:
 *   Le constructeur initialise le convertisseur.
 *   Une lecture enclanche une conversion et le resultat
 *   est retourne sur 16 bits.
 *
 */

class Capteur
{
public:

   capteur();
   capteur(Can);

   uint8_t getDistance();
 
private:
     void setValeurEx();
        uint8_t distance_;
   uint8_t valeurCan_=0;
   Can typeCan_;
};

#endif 

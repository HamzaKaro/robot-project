 
#include "motor.hpp"

Motor::Motor(Pin dirPin1, Pin dirPin2)
{
    dirPin1_ = dirPin1;
    dirPin2_ = dirPin2;

}

void Motor::setIntensity(int8_t pourcentA, int8_t pourcentB)
{
    if (pourcentA >= 0)
        dirPin1_.set(1);
    else
        dirPin1_.set(0);

    if (pourcentB >= 0)
        dirPin2_.set(1);
    else
        dirPin2_.set(0);

    if (pourcentA < 0)
        pourcentA *= -1;

    if (pourcentB < 0)
        pourcentB *= -1;

    OCR1A = pourcentA * (0xFFFE / 100);
    OCR1B = pourcentB * (0xFFFE / 100);

    
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << CS11);
    TCCR1C = 0;

    //_delay_ms(500);
}

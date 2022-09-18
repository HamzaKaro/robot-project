#pragma once

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
void _delay_ms(double ms);


enum Port {A, B, C, D};             // Port of the button on the microcontroller
enum Colour {GREEN, RED, GOLD, SHUT};   //State for a led


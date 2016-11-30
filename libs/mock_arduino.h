//
// Created by Alberto Gonzalez Perez on 29/11/16.
//

#ifndef HYPNOS_MOCK_ARDUINO_H
#define HYPNOS_MOCK_ARDUINO_H

#include <stdint.h>

typedef unsigned char byte;
typedef unsigned short int word;

uint32_t millis();
void delay(unsigned long ms);

void initialize_mock_arduino();


#endif //HYPNOS_MOCK_ARDUINO_H

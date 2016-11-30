//
// Created by Alberto Gonzalez Perez on 29/11/16.
//

#ifndef HYPNOS_WIRE_H
#define HYPNOS_WIRE_H

#include <cstdint>
#include "mock_arduino.h"

class TwoWire {

public:
    void begin();

    void requestFrom(int address, int bytes);

    bool available();

    byte read();
};

extern TwoWire Wire;

#endif //HYPNOS_WIRE_H

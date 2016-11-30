#include "Wire.h"

TwoWire Wire;

void TwoWire::begin() {

}

void TwoWire::requestFrom(int address, int bytes) {

}

bool TwoWire::available() {
    return false;
}

byte TwoWire::read() {
    return '\0';
}

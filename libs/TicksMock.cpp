//
// Created by Alberto Gonzalez Perez on 29/11/16.
//

#include "TicksMock.h"
#define MAH_PER_TICK 0.1707

TicksMock::TicksMock() {
    _ticks = 0;
}

void TicksMock::incrementTicks() {
    _ticks++;
}

void TicksMock::incrementTicks(int increment) {
    _ticks += increment;
}

uint32_t TicksMock::getTicks() {
    return _ticks;
}

void TicksMock::reset() {
    _ticks = 0;
}

uint32_t TicksMock::ticksNeeded(uint16_t batteryCapacity, float percentage) {
    return (uint32_t) ((batteryCapacity / MAH_PER_TICK) * (1.0 - percentage));
}

TicksMock Ticks;

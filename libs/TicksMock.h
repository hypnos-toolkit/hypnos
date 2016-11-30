//
// Created by Alberto Gonzalez Perez on 29/11/16.
//

#ifndef HYPNOS_TICKSMOCK_H
#define HYPNOS_TICKSMOCK_H

#include <cstdint>

class TicksMock {
public:
    TicksMock();
    void reset();
    void incrementTicks();
    void incrementTicks(int increment);
    uint32_t getTicks();
    uint32_t ticksNeeded(uint16_t batteryCapacity, float percentage);

private:
    uint32_t _ticks;
};

extern TicksMock Ticks;

#endif //HYPNOS_TICKSMOCK_H

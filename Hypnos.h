#ifndef Hypnos_h
#define Hypnos_h

#include "Arduino.h"
#include <Wire.h>

#define LONG_BYTES 4
#define BYTE 8
#define MAX_PERIOD 614 // See LTC4150 Datasheet

#define CONSUMPTION_COUNTER_ADDR 0x04
#define MAH_PER_TICK 0.1707

#define DEFAULT_MIN_DELAY 60000
#define DEFAULT_MAX_DELAY 43200000
#define DEFAULT_SLOPE 2
#define DEFAULT_DISPLACEMENT 20

class Hypnos {
  public:
    Hypnos(uint16_t batteryMAh);
    Hypnos(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis);
    void init();
    void setDelayFunction(void (*delayFunction)(uint32_t));
    void setMinDelayMillis(uint32_t minDelayMillis);
    void setMaxDelayMillis(uint32_t maxDelayMillis);
    void setSlope(uint16_t slope);
    void setDisplacement(uint32_t displacement);
    void setRequestDataFunction(uint32_t (*requestDataFunction)(uint8_t addr, uint8_t byte));
    uint32_t getConsumptionTicks();
    float getRemainingCapacity();
    float getRemainingPercentage();
    uint32_t previewSleepTime();
    void sleep();
  private:
    uint16_t _batteryMAh;
    void (*_delayFunction)(uint32_t);
    uint32_t (*_requestDataFunction)(uint8_t addr, uint8_t byte);
    uint32_t _lastTickCheck;
    uint32_t _lastTicks;
    uint32_t _min;
    uint32_t _max;
    uint32_t _slope;
    uint32_t _displacement;
    void _init(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis);
    uint32_t _getTicksFromCounter();
};

#endif

#ifndef Hypnos_h
#define Hypnos_h

#if defined(SPARK) || (PLATFORM_ID==88)
    #include "application.h"
    #include "math.h"
#elif defined(ARDUINO)
    #include <Arduino.h>
    #include <Wire.h>
#else
    #include <cstdint>
    #include <cmath>
    #include "libs/Wire.h"
    #include "libs/mock_arduino.h"
    #include "libs/TicksMock.h"
    #define TESTING_ENV
#endif

#define LONG_BYTES 4
#define BYTE 8
#define MAX_PERIOD 614 // See LTC4150 Datasheet

#define CONSUMPTION_COUNTER_ADDR 0x4
#define MAH_PER_TICK 0.1707

#define DEFAULT_MIN_DELAY 60000
#define DEFAULT_MAX_DELAY 43200000
#define DEFAULT_SLOPE 1.5
#define DEFAULT_DISPLACEMENT 20.0
#define UPPER_SLEEP_LIMIT 0.1

struct SleepData {
    uint16_t sleepCycleCounter;
    uint32_t sleepTimeStash;
    float batteryBeforeSleep;
};

class Hypnos {
  public:
    Hypnos(uint16_t batteryMAh);
    Hypnos(uint16_t batteryMAh, SleepData* sleepData);
    Hypnos(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis);
    Hypnos(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis, SleepData* sleepData);
    void init();
    void setDelayFunction(void (*delayFunction)(uint32_t));
    void setMinDelayMillis(uint32_t minDelayMillis);
    void setMaxDelayMillis(uint32_t maxDelayMillis);
    void setSlope(float slope);
    void setDisplacement(double displacement);
    uint32_t getConsumptionTicks();
    float getRemainingCapacity();
    float getRemainingPercentage();
    uint32_t previewSleepTime();
    void sleep();
  private:
    uint16_t _batteryMAh;
    void (*_delayFunction)(uint32_t);
    uint32_t _min;
    uint32_t _max;
    float _slope;
    double _displacement;
    void _init(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis);
    uint32_t _getTicksFromCounter();
    uint32_t _calculateSleepTimeOverThreshold(uint32_t threshold);
    void _putToSleepIfDischarging();
    uint32_t _lastTickCheck;
    uint32_t _lastTicks;
    SleepData* _sleepData;
};

#endif

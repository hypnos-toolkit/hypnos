#include "Hyperion.h"

Hyperion::Hyperion(uint16_t batteryMAh) {
  _init(batteryMAh, DEFAULT_MIN_DELAY, DEFAULT_MAX_DELAY);
}

Hyperion::Hyperion(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis) {
  _init(batteryMAh, minDelayMillis, maxDelayMillis);
}

void Hyperion::init() {
  Wire.begin();
}

void Hyperion::setDelayFunction(void (*delayFunction)(uint32_t)) {
  _delayFunction = delayFunction;
}

void Hyperion::setMinDelayMillis(uint32_t minDelayMillis) {
  _min = minDelayMillis;
}

void Hyperion::setMaxDelayMillis(uint32_t maxDelayMillis) {
  _max = maxDelayMillis;
}

void Hyperion::setSlope(uint16_t slope) {
  _slope = slope;
}

void Hyperion::setDisplacement(uint32_t displacement) {
  _displacement = displacement;
}

uint32_t Hyperion::getConsumptionTicks() {
  return _getTicksFromCounter();
}

float Hyperion::getRemainingCapacity() {
  return _batteryMAh - (_getTicksFromCounter() * MAH_PER_TICK);
}

float Hyperion::getRemainingPercentage() {
  return getRemainingCapacity() / _batteryMAh;
}

uint32_t Hyperion::previewSleepTime() {
  // Scale FROM -5 to 5
  float scaledBatteryLevel = getRemainingPercentage() * 10 - 5;

  // Biased sigmoid function that handicaps low battery levels
  float timelinePosition = 1.0 / (1.0 + _displacement * exp(_slope * scaledBatteryLevel));

  // Calculate delay time based on given parameters
  return (_max - _min) * timelinePosition + _min;
}

void Hyperion::sleep() {
  _delayFunction(previewSleepTime());
}

void Hyperion::_init(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis) {
  _batteryMAh = batteryMAh;

  _min = minDelayMillis;
  _max = maxDelayMillis;
  _slope = DEFAULT_SLOPE;
  _displacement = DEFAULT_DISPLACEMENT;

  _lastTickCheck = millis() - MAX_PERIOD;
  _lastTicks = 0; // TODO Initialize from NVRAM
}

uint32_t Hyperion::_getTicksFromCounter() {
  if (millis() >= _lastTickCheck + MAX_PERIOD) {
    uint8_t i = LONG_BYTES - 1;
    uint32_t ticks = 0;

    Wire.requestFrom(CONSUMPTION_COUNTER_ADDR, LONG_BYTES);
    while (Wire.available()) {
      ticks += ((uint8_t) Wire.read()) << i*BYTE;
      i--;
    }

    _lastTickCheck = millis();
    _lastTicks = ticks;
  }

  return _lastTicks;
}

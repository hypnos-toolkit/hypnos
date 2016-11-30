#include "Hypnos.h"

struct SleepData defSleepData = {false, 0, 0, false, 0.0};

Hypnos::Hypnos(uint16_t batteryMAh) {
  _init(batteryMAh, DEFAULT_MIN_DELAY, DEFAULT_MAX_DELAY, &defSleepData);
}

Hypnos::Hypnos(uint16_t batteryMAh, SleepData* sleepData) {
  _init(batteryMAh, DEFAULT_MIN_DELAY, DEFAULT_MAX_DELAY, sleepData);
}

Hypnos::Hypnos(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis) {
  _init(batteryMAh, minDelayMillis, maxDelayMillis, &defSleepData);
}

Hypnos::Hypnos(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis, SleepData* sleepData) {
  _init(batteryMAh, minDelayMillis, maxDelayMillis, sleepData);
}

void Hypnos::init() {
  Wire.begin();
  if (_sleepData->calledSleepFunctionOverThreshold) {
    _putToSleepIfDischarging();
  }
}

void Hypnos::setDelayFunction(void (*delayFunction)(uint32_t)) {
  _delayFunction = delayFunction;
}

void Hypnos::setMinDelayMillis(uint32_t minDelayMillis) {
  _min = minDelayMillis;
}

void Hypnos::setMaxDelayMillis(uint32_t maxDelayMillis) {
  _max = maxDelayMillis;
}

void Hypnos::setSlope(float slope) {
  _slope = slope;
}

void Hypnos::setDisplacement(double displacement) {
  _displacement = displacement;
}

uint32_t Hypnos::getConsumptionTicks() {
  return _getTicksFromCounter();
}

float Hypnos::getRemainingCapacity() {
  return (float) (_batteryMAh - (_getTicksFromCounter() * MAH_PER_TICK));
}

float Hypnos::getRemainingPercentage() {
  return getRemainingCapacity() / _batteryMAh;
}

uint32_t Hypnos::previewSleepTime() {
  // Scale FROM -5 to 5
  float scaledBatteryLevel = getRemainingPercentage() * 10 - 5;

  // Biased sigmoid function that handicaps low battery levels
  float timelinePosition = (float) (1.0f / (1.0f + _displacement * exp(_slope * scaledBatteryLevel)));

  // Calculate delay time based on given parameters
  return (uint32_t) ((_max - _min) * timelinePosition + _min);
}

void Hypnos::sleep() {
  uint32_t sleepTime = previewSleepTime();
  uint32_t threshold = (uint32_t) ((_max - _min) * UPPER_SLEEP_LIMIT + _min);

  if (sleepTime < threshold) {
      _delayFunction(sleepTime);
  } else {
    _sleepData->sleepCicleCounter++;
    sleepTime = _calculateSleepTimeOverThreshold((uint32_t) (log10(10 * _sleepData->sleepCicleCounter) * threshold));
    _sleepData->batteryBeforeSleep = getRemainingPercentage();
    _sleepData->calledSleepFunctionOverThreshold = true;
    _delayFunction(sleepTime);
    _putToSleepIfDischarging(); // Not called if sleep function resets board
  }
}

void Hypnos::_init(uint16_t batteryMAh, uint32_t minDelayMillis, uint32_t maxDelayMillis, SleepData* sleepData) {
  _batteryMAh = batteryMAh;

  _min = minDelayMillis;
  _max = maxDelayMillis;
  _slope = DEFAULT_SLOPE;
  _displacement = DEFAULT_DISPLACEMENT;

  _lastTickCheck = millis() - MAX_PERIOD;
  _lastTicks = 0; // TODO Initialize from NVRAM

  _sleepData = sleepData;
}

uint32_t Hypnos::_getTicksFromCounter() {
#ifdef TESTING_ENV
  return Ticks.getTicks();
#else
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
#endif
}

uint32_t Hypnos::_calculateSleepTimeOverThreshold(uint32_t threshold) {
  if (!_sleepData->insideSleepCheckCicle) {
    _sleepData->sleepTimeStash = previewSleepTime();
    _sleepData->insideSleepCheckCicle = true;
  }
  uint32_t sleepTime = (_sleepData->sleepTimeStash < threshold) ? _sleepData->sleepTimeStash : threshold;
  _sleepData->sleepTimeStash -= sleepTime;
  return sleepTime;
}

void Hypnos::_putToSleepIfDischarging() {
  _sleepData->calledSleepFunctionOverThreshold = false;

  float batteryAfterSleep = getRemainingPercentage();
  if (batteryAfterSleep < _sleepData->batteryBeforeSleep) { // If discharging continue sleeping
    if (_sleepData->insideSleepCheckCicle && _sleepData->sleepTimeStash == 0) {
      _sleepData->insideSleepCheckCicle = false;
      _sleepData->sleepCicleCounter = 0;
      return;
    }
    sleep();
  } else {
    _sleepData->insideSleepCheckCicle = false;
    _sleepData->sleepCicleCounter = 0;
  }
}

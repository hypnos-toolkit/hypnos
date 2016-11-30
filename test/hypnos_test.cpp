#include <cstdint>
#include "../Hypnos.h"
#include "gtest/gtest.h"
uint16_t CAPACITY = 100;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// ----------------------------------------------------------------
// DELAY HELPER
// ----------------------------------------------------------------

bool _called = false;
int _timesCalled = 0;
uint32_t _delayTime = 0;

void mockedDelay(uint32_t millis) {
    _called = true;
    _timesCalled++;
    _delayTime = millis;
}

bool mockedDelayWasCalled() {
    return _called;
}

uint32_t getMockedDelay() {
    return _delayTime;
}

int getTimesCalled() {
    return _timesCalled;
}

void resetMockedDelay() {
    _called = false;
    _delayTime = 0;
    _timesCalled = 0;
}

// ----------------------------------------------------------------

TEST(TicksMock, startsFromZero) {
    Ticks.reset();
    EXPECT_EQ(Ticks.getTicks(), 0);
}

TEST(TicksMock, canIncrementTicks) {
    Ticks.reset();
    Ticks.incrementTicks();
    EXPECT_EQ(Ticks.getTicks(), 1);
}

TEST(TicksMock, canIncrement2Ticks) {
    Ticks.reset();
    Ticks.incrementTicks(2);
    EXPECT_EQ(Ticks.getTicks(), 2);
}

TEST(TicksMock, calculatesTicks) {
    EXPECT_EQ(Ticks.ticksNeeded(CAPACITY, 0.5), 292);
}


TEST(TicksMock, returnsTicksForFullBattery) {
    EXPECT_EQ(Ticks.ticksNeeded(CAPACITY, 1.0), 0);
}

TEST(Hypnos, getsTotalCapacity) {
    Ticks.reset();
    Hypnos hypnos(CAPACITY);
    hypnos.init();
    EXPECT_EQ(hypnos.getRemainingCapacity(), CAPACITY);
}

TEST(Hypnos, getsConsumptionTicks) {
    Ticks.reset();
    Ticks.incrementTicks();
    Hypnos hypnos(CAPACITY);
    hypnos.init();
    EXPECT_EQ(hypnos.getConsumptionTicks(), 1);
}

TEST(Hypnos, getsRemainingPercentage) {
    Ticks.reset();
    Hypnos hypnos(CAPACITY);
    hypnos.init();
    EXPECT_EQ(hypnos.getRemainingPercentage(), 1.0);
}

TEST(Hypnos, callsDelay) {
    Ticks.reset();
    resetMockedDelay();

    Hypnos hypnos(CAPACITY);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();
    hypnos.sleep();
    EXPECT_TRUE(mockedDelayWasCalled());
}

TEST(Hypnos, previewsSleepTime) {
    Ticks.reset();
    resetMockedDelay();

    Hypnos hypnos(CAPACITY);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();
    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.5));
    EXPECT_EQ(hypnos.previewSleepTime(), 2069100);
}

TEST(Hypnos, sleepsForAWhile) {
    Ticks.reset();
    resetMockedDelay();

    Hypnos hypnos(CAPACITY);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();
    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.5));
    hypnos.sleep();
    EXPECT_EQ(getMockedDelay(), 2069100);
}

TEST(Hypnos, longSleepIsSmallerThanShortSleepDueTimePartition) {
    Ticks.reset();
    resetMockedDelay();

    Hypnos hypnos1(CAPACITY);
    hypnos1.setDelayFunction(&mockedDelay);
    hypnos1.init();
    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.45));
    hypnos1.sleep();
    uint32_t delay1 = getMockedDelay();

    Ticks.reset();
    resetMockedDelay();

    Hypnos hypnos2(CAPACITY);
    hypnos2.setDelayFunction(&mockedDelay);
    hypnos2.init();
    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.1));
    hypnos2.sleep();
    uint32_t delay2 = getMockedDelay();

    EXPECT_TRUE(delay2 < delay1);
}

TEST(Hypnos, callsSleepFunctionMoreThanOnceWithLowBatteryLevel) {
    Ticks.reset();
    resetMockedDelay();

    Hypnos hypnos(CAPACITY);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();
    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.1));
    hypnos.sleep();

    EXPECT_TRUE(getTimesCalled() > 1);
}

TEST(Hypnos, sleepsWithDischargingBatteryAndBeingInsideSleepCicle) {
    struct SleepData testSleepData = {true, 1, 20691000, true, 0.3};
    Ticks.reset();
    resetMockedDelay();

    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.2));
    Hypnos hypnos(CAPACITY, &testSleepData);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();

    EXPECT_TRUE(mockedDelayWasCalled());
}

TEST(Hypnos, doesNotSleepAfterRecoveringBattery) {
    struct SleepData testSleepData = {true, 1, 20691000, true, 0.3};
    Ticks.reset();
    resetMockedDelay();

    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.5));
    Hypnos hypnos(CAPACITY, &testSleepData);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();

    EXPECT_TRUE(!mockedDelayWasCalled());
}

TEST(Hypnos, getsOutOfSleepCicle) {
    struct SleepData testSleepData = {true, 1, 20691000, true, 0.3};
    Ticks.reset();
    resetMockedDelay();

    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.2));
    Hypnos hypnos(CAPACITY, &testSleepData);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();

    EXPECT_TRUE(!testSleepData.insideSleepCheckCycle);
}

TEST(Hypnos, emptiesSleepTimeStashAfterSleepCicle) {
    struct SleepData testSleepData = {true, 1, 20691000, true, 0.3};
    Ticks.reset();
    resetMockedDelay();

    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.2));
    Hypnos hypnos(CAPACITY, &testSleepData);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();

    EXPECT_EQ(testSleepData.sleepTimeStash, 0);
}

TEST(Hypnos, modifiesLastBatteryValue) {
    struct SleepData testSleepData = {true, 1, 20691000, true, 0.3};
    Ticks.reset();
    resetMockedDelay();

    Ticks.incrementTicks(Ticks.ticksNeeded(CAPACITY, 0.2));
    Hypnos hypnos(CAPACITY, &testSleepData);
    hypnos.setDelayFunction(&mockedDelay);
    hypnos.init();

    EXPECT_NEAR(testSleepData.batteryBeforeSleep, 0.2, 0.01);
}



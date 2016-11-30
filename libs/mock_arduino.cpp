#include <sys/timeb.h>
#include "mock_arduino.h"

timeb t_start;
uint32_t millis() {
    timeb t_now;
    ftime(&t_now);
    return (uint32_t) ((t_now.time - t_start.time) * 1000 + (t_now.millitm - t_start.millitm));
}

void delay(unsigned long ms) {
    unsigned long start = millis();
    while(millis() - start < ms){}
}

void initialize_mock_arduino() {
    ftime(&t_start);
}
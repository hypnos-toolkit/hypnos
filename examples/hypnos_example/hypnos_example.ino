#include <Hypnos.h>
#define BATTERY_MAH 1050 // Change this in accordance to your battery spec

Hypnos hypnos(BATTERY_MAH);

void setup() {
  Serial.begin(9600);
  hypnos.setMinDelayMillis(1000); // By default 1min
  hypnos.setMaxDelayMillis(10*3600*1000); // By default 12h
  hypnos.setDelayFunction(delay);
  hypnos.init(); // Important this must go after all setter statements
}

void loop() {
  Serial.print("Ticks: ");
  Serial.print(hypnos.getConsumptionTicks());
  Serial.print(", Remaining mAh: ");
  Serial.print(hypnos.getRemainingCapacity());
  Serial.print(", Remaining %: ");
  Serial.print(hypnos.getRemainingPercentage() * 100.0);
  Serial.print(", Sleep time: ");
  Serial.println(hypnos.previewSleepTime());
  hypnos.sleep();
}

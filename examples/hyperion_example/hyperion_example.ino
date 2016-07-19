#include <Hyperion.h>
#define BATTERY_MAH 1050 // Change this in accordance to your battery spec

Hyperion hyperion(BATTERY_MAH);

void setup() {
  hyperion.init();
  Serial.begin(9600);
  hyperion.setMinDelayMillis(1000); // By default 1min
  hyperion.setMaxDelayMillis(10*3600*1000); // By default 12h
  hyperion.setDelayFunction(delay);
}

void loop() {
  Serial.print("Ticks: ");
  Serial.print(hyperion.getConsumptionTicks());
  Serial.print(", Remaining mAh: ");
  Serial.print(hyperion.getRemainingCapacity());
  Serial.print(", Remaining %: ");
  Serial.print(hyperion.getRemainingPercentage() * 100.0);
  Serial.print(", Sleep time: ");
  Serial.println(hyperion.previewSleepTime());
  hyperion.sleep();
}

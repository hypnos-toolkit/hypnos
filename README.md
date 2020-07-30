# Hypnos

Hypnos is a hardware and software toolkit which facilitates the creation of energy-aware IoT nodes based on widely extended consumer board platforms: Arduino, LinkIt One and Particle, to mention a few.

The present repository contains the library source code which jointly works with the [Hypnos Board](https://github.com/hypnos-toolkit/hypnos-board). However, if your system already includes a reliable mechanism to obtain the remaininig battery charge, feel free to adapt the code in order to use it instead.

# Usage sample ([source](./examples/hypnos_example/hypnos_example.ino))

```ino
#include <Hypnos.h>
#define BATTERY_MAH 1050 // Change this in accordance to your battery spec

Hypnos hypnos(BATTERY_MAH);

void setup() {
  Serial.begin(9600);
  hypnos.setMinDelayMillis(1000); // By default 1min
  hypnos.setMaxDelayMillis(10*3600*1000); // By default 12h
  hypnos.setDelayFunction(&delay);
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
```

# Compatibility

The [board](https://github.com/hypnos-toolkit/hypnos-board), in conjunction with this C ++ library, has been successfully tested on the following microcontroller boards:
* Arduino UNO R3
* SeeedStudio LinkIt One
* Particle Photon (via a Shield-Shield adapter)

#MR60BHA1 Example

```
#include <Arduino.h>
#include <mr60bha1.h>

MR60BHA1 radar;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("RadarModul v0.0a");
  radar.setup();
}

void loop()
{
  static uint32_t old_ms = 0;

  radar.loop();

  uint32_t ms = millis();

  if(ms >= (old_ms + 2000))
  {
    old_ms = ms;

    uint16_t human_distance = radar.get_human_distance();
    uint8_t heart_rate = radar.get_heart_rate();
    uint8_t respiratory_rate = radar.get_respiratory_rate();
    uint8_t respiratory_value = radar.get_respiratory_value();

    Serial.printf("Distance: %d\nHeartRate: %d\nRespiratoryRate: %d\nRespiratoryValue: %d\n\n", human_distance, heart_rate, respiratory_rate, respiratory_value);
  }
}
```
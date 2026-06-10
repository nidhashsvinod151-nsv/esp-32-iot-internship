markdown# SensorBundle Struct — Zelbytes IoT Lab Field Mapping

## Overview
SensorBundle is the standard data structure used to carry
DHT22 sensor readings through the firmware pipeline.
Field names match Zelbytes IoT Lab JSON keys directly,
ensuring consistent data ingestion from Day 5 through
Day 7 (lab dashboard) and Day 12 (MQTT publish).

---

## Struct Definition

```cpp
struct SensorBundle {
  float temperature;       // JSON: "temp_c"
  float humidity;          // JSON: "humidity_pct"
  float heatIndex;         // JSON: "heat_index_c"
  bool  valid;             // JSON: "valid"
  unsigned long timestamp; // JSON: "timestamp_ms"
  int   checksumFails;     // JSON: "checksum_fails"
};
```

---

## Field Mapping Table

| Struct Field  | JSON Key       | Type   | Unit      | Notes                          |
|---------------|----------------|--------|-----------|--------------------------------|
| temperature   | temp_c         | float  | °C        | Median filtered, ±0.5°C acc.   |
| humidity      | humidity_pct   | float  | % RH      | Relative humidity              |
| heatIndex     | heat_index_c   | float  | °C        | Computed from temp + humidity  |
| valid         | valid          | bool   | true/false| false = all 3 retries failed   |
| timestamp     | timestamp_ms   | ulong  | ms        | millis() since boot            |
| checksumFails | checksum_fails | int    | count     | Cumulative bad readings        |

---

## Example JSON Payload

```json
{
  "node_id"        : "esp32-lab-01",
  "temp_c"         : 28.40,
  "humidity_pct"   : 65.20,
  "heat_index_c"   : 30.10,
  "valid"          : true,
  "timestamp_ms"   : 4023,
  "checksum_fails" : 0
}
```

---

## Error Handling Policy

| Condition                | valid  | Action                           |
|--------------------------|--------|----------------------------------|
| Clean reading            | true   | Add to filter, publish           |
| isnan on attempt 1 or 2  | false  | Log error, retry (max 3)         |
| All 3 attempts failed    | false  | Log error, skip cycle, continue  |
| Spike detected by filter | true   | Median filter corrects value     |

---

## Median Filter

Window size: 3 last good samples.
Raw samples : [28.4, 34.2, 28.5]   ← 34.2 is breath spike
After sort  : [28.4, 28.5, 34.2]
Median      : 28.5  ← spike rejected ✅

---

## Sensor Specifications

| Parameter         | Value              |
|-------------------|--------------------|
| Model             | DHT22 (AM2302)     |
| Temperature range | -40°C to +80°C     |
| Humidity range    | 0% to 100% RH      |
| Accuracy (temp)   | ±0.5°C             |
| Accuracy (hum)    | ±2–5% RH           |
| Sample interval   | Minimum 2 seconds  |
| Logic voltage     | 3.3V only (ESP32)  |
| Pull-up resistor  | 10kΩ DATA to 3.3V  |
| Protocol          | Single-wire        |

---

## Used In
- Day 5 : Sensor wiring and validation
- Day 7 : Zelbytes IoT Lab dashboard feed
- Day 12: MQTT publish payload

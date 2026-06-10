---

## Day 5 — DHT22 Sensor + Median Filter

### Overview
DHT22 wired and validated for polyhouse climate monitoring.
Readings feed Zelbytes IoT Lab dashboard on Day 7 and
MQTT broker on Day 12. Robust error handling implemented
to prevent pipeline failures in later stages.

### Wiring
| DHT22 Pin | ESP32 Pin | Notes                      |
|-----------|-----------|----------------------------|
| VCC       | 3.3V      | Never use 5V on ESP32      |
| DATA      | GPIO4     | 10kΩ pull-up to 3.3V       |
| NC        | —         | Leave unconnected          |
| GND       | GND       |                            |

### Error Handling
- `isnan()` check on every read
- Up to 3 retries per cycle before skipping
- Failures logged to serial — loop() never crashes
- Checksum fail count tracked in SensorBundle

### Median Filter — Breath Test Result
- Filter window: 3 good samples
- Breath spike (~34°C) successfully rejected ✅
- Filtered output stayed within ±0.1°C of baseline ✅

### Files Added — Day 5
| File | Purpose |
|------|---------|
| `week1-day05/day05_dht22_sensor.ino` | DHT22 sketch with retry and median filter |
| `docs/sensor_bundle.md` | SensorBundle JSON field mapping for Zelbytes |

### Day 5 Checklist
- [x] DHT22 wired with pull-up, 3.3V logic only ✅
- [x] Valid readings within ±0.5°C of reference ✅
- [x] Checksum failures logged without watchdog reset ✅
- [x] Median filter rejects single sample spike ✅
- [x] SensorBundle struct documented for Zelbytes ✅

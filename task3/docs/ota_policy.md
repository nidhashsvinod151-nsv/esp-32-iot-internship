# OTA Update Policy — Zelbytes IoT Lab

## What is OTA?
Over-The-Air (OTA) update allows new firmware to be flashed
to the ESP32 wirelessly over WiFi without a USB cable.
Essential for deployed polyhouse nodes where physical
access is inconvenient.

---

## Partition Layout

Default with OTA (Minimal SPIFFS 1.9MB):
├── app0  (running firmware)
├── app1  (OTA update target)
└── spiffs (file storage)

On successful OTA the bootloader switches to the new slot.
If new firmware crashes it rolls back to previous slot.

---

## Lab Environment

| Parameter      | Value                     |
|----------------|---------------------------|
| Hostname       | esp32-zelbytes-lab        |
| OTA password   | Set via OTA_PASSWORD      |
| Open OTA       | Rejected — auth required  |
| Partition      | Minimal SPIFFS with OTA   |
| Network        | Same WiFi as upload PC    |

---

## OTA Flash Results

| Attempt      | Method          | Result      |
|--------------|-----------------|-------------|
| First flash  | USB cable COM5  | ✅ Success  |
| Second flash | OTA WiFi        | ✅ Success  |
| Wrong password| OTA WiFi       | ✅ Rejected |

---

## Production Signing Requirements

### Lab (Current)

Password authentication only
Plain WiFi UDP transport
Manual rollback if needed


### Production (Recommended)

Signed firmware binary (RSA key pair)
HTTPS transport for firmware delivery
Automatic rollback on crash
Version number enforcement
Certificate pinning


---

## OTA Error Codes

| Error             | Meaning           | Fix                    |
|-------------------|-------------------|------------------------|
| OTA_AUTH_ERROR    | Wrong password    | Check OTA_PASSWORD     |
| OTA_BEGIN_ERROR   | No flash space    | Check partition scheme |
| OTA_CONNECT_ERROR | Can't reach board | Check same WiFi network|
| OTA_RECEIVE_ERROR | Transfer failed   | Retry upload           |
| OTA_END_ERROR     | Finalise failed   | Check flash integrity  |

---

## Critical Rule

Always call `ArduinoOTA.handle()` as first line in loop():

```cpp
void loop() {
  ArduinoOTA.handle(); // Must be first — never skip
  // rest of code
}
```

Missing this makes OTA silently unavailable.

# Day 19 - Edge Node Configuration Management

## Overview
Replaced hard-coded `secrets.h` values with persistent configuration stored
in ESP32 NVS flash (via the Preferences library), under the namespace
`polyhouse`. This allows WiFi credentials, MQTT broker address, node ID,
sample interval, and calibration offsets to be updated at runtime without
reflashing firmware - necessary for managing multiple polyhouse nodes at
scale.

## Implementation
- Config schema versioned (`config_schema_version`) so future firmware
  updates can detect and migrate older stored configs.
- On boot, config is loaded from NVS before WiFi.connect(); if NVS is
  empty (first boot / factory-reset state), the node falls back to a
  provisioning mode instead of connecting with blank credentials.
- `set_interval` MQTT command (from Day 13's command handler) now persists
  the new sample interval directly into NVS, surviving power cycles.
- Factory reset implemented via GPIO button held for 5 seconds, which
  clears the entire NVS `polyhouse` namespace and returns the node to
  provisioning fallback on next boot.
- Serial command `dump_config` added for field debugging - prints all
  non-secret config fields (WiFi password is intentionally excluded from
  any serial output, gated behind `#ifdef DEBUG` per mentor guidance).

## Results
- Config confirmed to persist across reboot (changed SSID in NVS, device
  reconnected using the new value after reset).
- `set_interval` command via MQTT updates NVS and the new interval is
  retained after a power cycle.
- Factory reset (5s button hold) successfully clears NVS; node enters
  provisioning fallback as expected.
- `dump_config` serial command prints all non-secret fields correctly.

## Files
- `firmware/day19_config_nvs.ino` - Config load/save/factory-reset logic
- `docs/config_schema.md` - Documented config JSON schema, keys, and
  defaults
- `firmware/secrets.template.h` - Redacted credential template (used only
  for first-time provisioning, not for runtime config anymore)
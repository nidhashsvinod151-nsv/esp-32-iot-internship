# Day 13 - MQTT Subscribe & Remote Commands

## Overview
Implemented MQTT command handler on ESP32 for remote control of polyhouse
actuators (irrigation valve) and configuration (sampling interval).

## Note on Implementation
Zelbytes IoT Lab REST API supports publishing to MQTT topics
(POST /mqtt/publish) but has no GET endpoint to read command topic messages.
To demonstrate the callback/handler logic, commands were injected via
Serial Monitor input (simulating MQTT message arrival), while ACK responses
were published for real to `zelbytes/lab/92/command/ack` via HTTPS
(confirmed HTTP 202).

## Results
- `IRRIGATE` command → valve OPEN action logged
- `STOP` command → valve CLOSE action logged
- `set_interval` command updates publish interval variable
- Duplicate `command_id` correctly ignored (idempotency table)
- Unknown command rejected with error ACK
- All ACKs published successfully (HTTP 202)

## Files
- `firmware/day13_mqtt_subscribe.ino` —
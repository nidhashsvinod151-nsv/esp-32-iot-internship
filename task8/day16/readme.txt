# Zelbytes IoT Lab API Reference

## Instance Details
- API Base: https://careers.zelbytes.com/api/iot-lab/v1
- Device/Node ID: 91
- Auth header: X-Iot-Lab-Key

## Endpoints
- Telemetry (HTTPS POST): /telemetry
- Telemetry (latest): /telemetry/latest
- Telemetry (series): /telemetry/series
- Telemetry (history): /telemetry/history
- MQTT publish: /mqtt/publish
- MQTT topic: zelbytes/lab/91/telemetry
- CoAP resources: /coap

## SensorBundle Payload (schema_version 1)
Example real payload sent successfully on Day 16:
{"schema_version":1,"node_id":"91","ts":2,"temp_c":31.2,"humidity_pct":87.8,"time_valid":false,"fw_version":"1.0.0"}

Payload size: 116 bytes (well under 512-byte limit)

## Response Codes Observed
- 202: Accepted (confirmed working, returns {status, protocol, id, recorded_at})
- 403: Invalid/missing API key (tutorial brief mentions 401; actual server
  returns 403 for bad keys — documented discrepancy)
- 422: Schema invalid (not yet triggered in testing)
- 5xx / -1: Transient/connection error — retried up to 3x with exponential
  backoff (1s, 2s, 4s)

## Known Note
`ts` field currently uses millis()/1000 as a placeholder since this fresh
sketch does not yet include NTP sync. `time_valid` is correctly set to
false to reflect this. Real epoch timestamp should be wired in before
production use.

## Status
Endpoint live and confirmed working as of 2026-06-17, multiple successful
202 responses recorded (ids 2605, 2611).
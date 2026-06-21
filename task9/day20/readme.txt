# Day 20 - Field Deployment Checklist

## Overview
Code that runs reliably on a bench can still fail once mounted in the real
polyhouse environment - 12 m scaffolding, metal framing (RF interference),
mist (moisture ingress), and a shared farm WiFi network. This session
produces a field deployment checklist and runs a mock deployment in the
lab to validate the node before any real installation.

## Implementation
- Created a pre-deployment checklist (20+ items) covering: firmware
  version confirmation, WiFi RSSI survey, IP plan, MQTT ACL verification,
  physical sensor placement guidelines, lightning/ESD grounding, and a
  maintenance schedule.
- Performed a mock deployment in the lab: DHT22 mounted inside an
  enclosure, node powered from a USB power bank (simulating field power
  conditions, no bench supply), and left running for a soak test to
  confirm stability over time.
- Logged WiFi signal strength (RSSI) at the simulated install location to
  validate connectivity margin before physical deployment.
- Documented a rollback procedure describing the steps to take if a node
  stops publishing telemetry after deployment (diagnosis steps, fallback
  actions, and recovery).
- Photographed the enclosure with labeled wiring for field reference and
  mentor review.

## Results
- Field checklist completed with 20+ verified pass/fail items
  (`docs/field_deployment_checklist.md`).
- WiFi RSSI logged at the install location, confirming acceptable signal
  margin for reliable connectivity.
- Soak test completed and logged (24 h field target, compressed to a
  shorter lab-equivalent run) with no missed telemetry intervals or
  crashes observed.
- Rollback runbook written describing recovery steps if the node goes
  silent post-deployment.
- Enclosure photo captured showing DHT22 mounting and labeled wiring.

## Files
- `docs/field_deployment_checklist.md` - 20+ item pre-deployment checklist
  with pass/fail columns
- `docs/wifi_rssi_survey.md` - RSSI readings/table at install location
- `docs/soak_test_log.md` - Soak test duration, interval, and results
- `docs/rollback_runbook.md` - Recovery procedure if node stops publishing
- `images/enclosure_wiring.jpg` - Labeled enclosure photo
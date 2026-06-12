# Day 10 — MQTT Protocol & Pub/Sub

**Track:** ESP32 IoT Internship | **Type:** Concept + CLI lab

## What is MQTT?
Lightweight pub/sub protocol for IoT. A **broker** routes messages between **publishers** and **subscribers** using **topics** — devices never talk directly.

## Topic Hierarchy
```
zelbytes/polyhouse/{node_id}/telemetry
zelbytes/polyhouse/{node_id}/status
zelbytes/polyhouse/{node_id}/cmd/reset
zelbytes/polyhouse/+/telemetry        # all nodes
```

## QoS Levels
| Level | Guarantee |
|-------|-----------|
| QoS 0 | Fire and forget |
| QoS 1 | At least once |
| QoS 2 | Exactly once |

## CLI Used
```bash
mosquitto_sub -h localhost -t "zelbytes/polyhouse/+/telemetry" -v
mosquitto_pub -h localhost -t "zelbytes/polyhouse/node-01/telemetry" -m '{"temp_c":28.5}' -q 1
```

## Key Terms
- **Retained** — broker saves last message; new subscribers get it instantly
- **LWT** — broker publishes a message if device disconnects unexpectedly

> Next: Day 11 — ESP32 PubSubClient first publish

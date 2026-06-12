
## Day 6 — OTA Updates and Deep Sleep

### Overview
Implemented Over-The-Air firmware update on ESP32 for
wireless code deployment without USB cable. Deep sleep
snippet saved for capstone power optimization.

### OTA Flash Results
| Attempt       | Method         | Result      |
|---------------|----------------|-------------|
| First flash   | USB cable COM5 | ✅ Success  |
| Second flash  | OTA WiFi       | ✅ Success  |
| Wrong password| OTA Network    | ✅ Rejected |

### Deep Sleep Power Comparison
| Mode        | Current Draw |
|-------------|--------------|
| Active WiFi | ~240 mA      |
| Modem sleep | ~20 mA       |
| Light sleep | ~2 mA        |
| Deep sleep  | ~0.01 mA     |

### Files Added — Day 6
| File | Purpose |
|------|---------|
| `week1-day06/day06_ota_update.ino` | OTA update manager sketch |
| `docs/ota_policy.md` | Lab vs production OTA policy |
| `docs/deep_sleep_snippet.ino` | Deep sleep timer for capstone |

### Day 6 Checklist
- [x] Partition scheme supports OTA ✅
- [x] First USB flash succeeded ✅
- [x] Second flash via OTA Network Port succeeded ✅
- [x] OTA password set, open OTA rejected ✅
- [x] docs/ota_policy.md completed ✅
- [x] Deep sleep snippet saved ✅

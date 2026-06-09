---

## Day 3 — WiFi Credentials & Connection

### Objectives
- Securely manage WiFi credentials using `secrets.h`
- Connect ESP32 to WiFi within 15 seconds
- Log IP address, RSSI, and channel on successful connection
- Implement exponential backoff reconnection logic
- Document WiFi provisioning methods

---

### Credentials Management

WiFi credentials are stored in a separate file `secrets.h`
which is excluded from Git using `.gitignore`.
This prevents sensitive data from being pushed to GitHub.

**secrets.h structure:**
```cpp
#ifndef SECRETS_H
#define SECRETS_H

const char* WIFI_SSID     = "YourWiFiName";
const char* WIFI_PASSWORD = "YourWiFiPassword";

#endif
```

**Note:** Never commit secrets.h to GitHub.
The .gitignore file handles this automatically.

---

### WiFi Connection — Serial Output

On successful connection the serial monitor prints:

### RSSI Signal Strength Guide

| RSSI Value      | Signal Quality |
|-----------------|----------------|
| -30 to -50 dBm  | Excellent       |
| -50 to -70 dBm  | Good            |
| -70 to -80 dBm  | Weak            |
| Below -80 dBm   | Very poor       |

---

### Exponential Backoff Reconnection

If WiFi drops, the node retries with increasing delays
to avoid flooding the network with reconnect attempts.


Formula used:
```cpp
int backoff = min(1000 * (1 << retryCount), 16000);
```

---

### Issue Faced — Special Character in SSID

During Day 3 testing the hotspot name contained a special
apostrophe character `'` (curly quote) in the SSID:


**Root cause:** The ESP32 WiFi library could not correctly
parse the curly apostrophe character in the SSID string.

**Fix:** Renamed the hotspot to remove the apostrophe:


**Lesson learned:** Always use plain alphanumeric characters
in hotspot/WiFi names when working with embedded systems.

---

### Files Added — Day 3

| File | Purpose |
|------|---------|
| `week1-day03/day03_wifi_connect.ino` | Main WiFi connection sketch |
| `secrets.h` | WiFi credentials (not committed) |
| `.gitignore` | Excludes secrets.h from Git |
| `docs/wifi_provisioning.md` | SmartConfig vs captive portal comparison |

---

### WiFi Provisioning Methods — Summary

| Method         | App Required | Reliability | Best For          |
|----------------|--------------|-------------|-------------------|
| SmartConfig    | Yes          | Medium      | Factory setup     |
| Captive Portal | No (browser) | High        | Field deployment  |
| secrets.h      | No           | High        | Lab/dev only      |

Full comparison: [docs/wifi_provisioning.md](docs/wifi_provisioning.md)

---

### Day 3 Checklist

- [x] secrets.h created and listed in .gitignore ✅
- [x] Node connects to WiFi within 15s ✅
- [x] Serial log shows IP, RSSI, and channel ✅
- [x] Disconnect test with backoff retry working ✅
- [x] docs/wifi_provisioning.md written ✅

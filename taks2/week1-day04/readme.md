---

## Day 4 — NTP Time Sync

### Overview
Day 4 focused on synchronizing the ESP32 internal clock
with internet time servers using NTP (Network Time Protocol).
Since the ESP32 has no built-in RTC battery it loses time
on every restart. NTP fixes this automatically after every
boot once WiFi is connected.

---

### NTP Configuration

```cpp
const char* NTP_SERVER1      = "pool.ntp.org";
const char* NTP_SERVER2      = "time.google.com";
const long  GMT_OFFSET_SEC   = 19800;  // IST = UTC+5:30
const int   DAYLIGHT_OFFSET_SEC = 0;   // India has no daylight saving
```

**Why IST offset = 19800?**
5 hours × 3600 seconds = 18000
30 minutes × 60 seconds =  1800
Total                   = 19800 seconds

---

### NTP Sync — Serial Output

On successful sync the serial monitor prints:
========================
Day 04 - NTP Time Sync
Connecting to WiFi........
WiFi connected ✓
IP     : 192.168.x.x
RSSI   : -52 dBm
Channel: 6
Syncing NTP..........
NTP sync successful ✓
Date      : 08/06/2026
Time      : 11:32:45
ISO       : 2026-06-08T11:32:45+05:30

---

### isoTimestamp() Function

Returns time in ISO 8601 format directly usable in JSON payloads
and consistent with Zelbytes IoT Lab timestamp requirements.

```cpp
String isoTimestamp() {
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    return "1970-01-01T00:00:00+05:30"; // fallback
  }
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S+05:30", &timeInfo);
  return String(buffer);
}
```

**Example output:**
2026-06-08T11:32:45+05:30

**Fallback when NTP not synced:**
1970-01-01T00:00:00+05:30

---

### Time Verification Against Phone Clock

| Source       | Time Shown | Match |
|--------------|------------|-------|
| Phone clock  | 11:32:47   | ✅    |
| ESP32 serial | 11:32:45   | ✅    |

Difference of 1-2 seconds is normal due to NTP sync delay.

---

### Combined WiFi + NTP Sketch Flow
Boot
│
├─→ Connect WiFi (15s timeout)
│     ├─→ Success → Sync NTP (10s timeout)
│     │     ├─→ Success → Print time every 5s
│     │     └─→ Failed  → Log error, use fallback timestamp
│     └─→ Failed → Log error, stop
│
└─→ Loop
├─→ WiFi connected    → Print time every 5s
└─→ WiFi disconnected → Exponential backoff retry
→ Reconnect WiFi
→ Resync NTP after reconnect

---

### NTP Servers Used

| Server          | Type          | Notes                  |
|-----------------|---------------|------------------------|
| pool.ntp.org    | Public pool   | Primary, global pool   |
| time.google.com | Google public | Backup, very reliable  |

**Port used:** UDP port 123

---

### NTP Blocked by Firewall — Behavior and Retry Policy

**Symptoms when NTP is blocked:**
- `getLocalTime()` always returns false
- Serial shows dots with no sync confirmation
- Timeout message after 10 seconds
- `isoTimestamp()` returns `1970-01-01T00:00:00+05:30`

**Common reasons for blocking:**
- Corporate or lab firewalls block UDP port 123
- Strict network policies prevent external DNS
- Captive portals intercept all outbound traffic

**Retry policy implemented:**

Attempt NTP sync for 10 seconds on boot
If failed → log error, continue with fallback timestamp
On every WiFi reconnect → retry NTP sync automatically
Two NTP servers configured so if one is blocked
the other may still succeed


**Recommended improvements for production:**

Add DS3231 RTC module as hardware backup
Cache last known good timestamp in flash memory
Accept time correction from MQTT broker
Trigger LED alert if timestamp is more than 24h stale


Full documentation: [docs/ntp_retry_policy.md](docs/ntp_retry_policy.md)

---

### Files Added — Day 4

| File | Purpose |
|------|---------|
| `week1-day04/day04_ntp_time.ino` | Combined WiFi + NTP sketch |
| `docs/ntp_retry_policy.md` | NTP firewall behavior and retry policy doc |

---

### Day 4 Checklist

- [x] NTP sync within 10s after WiFi connect ✅
- [x] Serial prints correct local time (IST) ✅
- [x] isoTimestamp() returns JSON-ready string ✅
- [x] NTP integrated into combined WiFi + NTP sketch ✅
- [x] NTP blocked behavior and retry policy documented ✅

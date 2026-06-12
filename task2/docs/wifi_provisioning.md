# WiFi Provisioning Methods

## What is WiFi Provisioning?
Provisioning is how an IoT device receives WiFi credentials
(SSID and password) without hardcoding them. This is important
for devices deployed in different locations.

---

## Method 1: SmartConfig

### How it works
A smartphone app sends WiFi credentials to the ESP32 over
the air using UDP broadcast packets. The ESP32 listens in
promiscuous mode and picks up the credentials.

### Pros
- No need to connect to the device directly
- Works entirely from a phone app
- Fast setup (under 30 seconds)

### Cons
- Requires a specific app (e.g. ESP Touch)
- Can fail on 5GHz networks (ESP32 is 2.4GHz only)
- Less reliable on busy networks

### Best for
Factory provisioning or when the device has no display.

---

## Method 2: Captive Portal

### How it works
The ESP32 creates its own temporary WiFi hotspot. You connect
to it from your phone/laptop and a login-style page opens
automatically. You enter your real WiFi credentials there.
The ESP32 saves them and connects to your network.

### Pros
- Works with any device that has a browser
- No special app needed
- More reliable than SmartConfig
- User-friendly interface

### Cons
- Takes slightly longer to set up
- Requires user to switch WiFi networks temporarily

### Best for
Consumer products or field deployment where users set up
their own devices.

---

## Comparison Table

| Feature            | SmartConfig        | Captive Portal     |
|--------------------|--------------------|--------------------|
| App required       | Yes (ESP Touch)    | No (just browser)  |
| Setup time         | ~20 seconds        | ~1 minute          |
| Reliability        | Medium             | High               |
| 5GHz support       | ❌                 | ✅                 |
| User friendliness  | Medium             | High               |
| Best use case      | Factory setup      | Field deployment   |

---

## What we used in this project
For this internship we used **hardcoded credentials via secrets.h**
because we are working in a controlled lab environment. In a
real deployment we would use Captive Portal for its reliability
and ease of use.

---

## References
- ESP32 SmartConfig docs: docs.espressif.com
- WiFiManager library (Captive Portal): github.com/tzapu/WiFiManager

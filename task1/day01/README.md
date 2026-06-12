
# ESP32 IoT Internship

## Board Decision: ESP32 vs ESP8266

| Feature        | ESP8266     | ESP32        |
|----------------|-------------|--------------|
| WiFi           | ✅          | ✅           |
| Bluetooth      | ❌          | ✅           |
| GPIO Pins      | 11          | 30+          |
| Processing     | Single core | Dual core    |
| Price          | Cheaper     | Slightly more|

**I chose [ESP32 / ESP8266] because:**
[Write your reason here]

## DHT22 Pin Map

| DHT22 Pin | ESP32 Pin | Notes              |
|-----------|-----------|--------------------|
| VCC       | 3.3V      | Do NOT use 5V      |
| DATA      | GPIO4     | 10kΩ pull-up needed|
| NC        | —         | Leave unconnected  |
| GND       | GND       |                    |

## Weekly Progress
- [x] Week 1: Arduino core installed ✅
- [x] Week 1: USB driver installed ✅
- [x] Week 1: Blink sketch uploaded ✅
- [ ] Week 1: DHT22 wiring documented
- [ ] Week 1: README completed

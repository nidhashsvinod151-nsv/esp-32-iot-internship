## Partition Scheme
- Selected: Default with OTA (3MB APP / 1MB SPIFFS)
- Reason: Allows wireless firmware updates (OTA) without
  needing USB cable every time



## Libraries Used
| Library       | Author           | Purpose              |
|---------------|------------------|----------------------|
| DHT           | Adafruit         | Read DHT22 sensor    |
| PubSubClient  | Nick O'Leary     | MQTT communication   |
| ArduinoJson   | Benoit Blanchon  | Parse/build JSON data|

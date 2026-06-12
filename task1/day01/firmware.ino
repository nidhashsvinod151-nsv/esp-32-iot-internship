// src/day02_env_check.ino
#include <Arduino.h>

#if !defined(ESP32) && !defined(ESP8266)
#error "Select ESP32 or ESP8266 board in Arduino IDE / PlatformIO"
#endif

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println(F("=== WiFi Edge Node Build Check ==="));
#ifdef ESP32
  Serial.printf("Arduino-ESP32 core: %s\n", ARDUINO_ESP32_GIT_DESC);
  Serial.printf("SDK: %s\n", ESP.getSdkVersion());
#endif
#ifdef ESP8266
  Serial.printf("Arduino-ESP8266 core: %s\n", ARDUINO_ESP8266_GIT_DESC);
#endif
  Serial.printf("CPU freq: %u MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
  Serial.println(F("Libraries OK if this compiles with DHT, PubSubClient, ArduinoJson installed."));
}

void loop() {
  delay(5000);
  Serial.printf("Heap: %u\n", ESP.getFreeHeap());
}

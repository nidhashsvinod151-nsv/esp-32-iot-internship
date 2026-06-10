#include <WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("--- STARTING ---");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 15000) {
      Serial.println("\nWiFi FAILED!");
      Serial.print("Status code: ");
      Serial.println(WiFi.status());
      return;
    }
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  ArduinoOTA.setHostname("esp32-zelbytes-lab");
  ArduinoOTA.setPassword("zelbytes2026");
  ArduinoOTA.begin();
  Serial.println("OTA ready!");
}

void loop() {
  ArduinoOTA.handle();
  Serial.println("Loop running - OTA v2!");
  delay(5000);
}

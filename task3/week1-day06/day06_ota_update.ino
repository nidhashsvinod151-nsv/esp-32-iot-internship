#include <WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"

#define OTA_HOSTNAME  "esp32-zelbytes-lab"
#define OTA_PASSWORD  "zelbytes2026"

int otaProgress = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("================================");
  Serial.println("  Day 06 - OTA Update Manager");
  Serial.println("  Zelbytes IoT Lab");
  Serial.println("================================");

  connectWiFi();
  setupOTA();

  Serial.println("running..");
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  unsigned long startTime = millis();

  wh

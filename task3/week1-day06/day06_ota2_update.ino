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



  #define uS_TO_S_FACTOR 1000000ULL
#define SLEEP_DURATION_SEC 30

RTC_DATA_ATTR int bootCount = 0;

void printWakeupReason() {
  esp_sleep_wakeup_cause_t wakeup = esp_sleep_get_wakeup_cause();
  switch (wakeup) {
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup: timer");
      break;
    default:
      Serial.println("Wakeup: power on or reset");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  bootCount++;
  Serial.print("Boot count: ");
  Serial.println(bootCount);

  printWakeupReason();

  // Add sensor read and MQTT publish here for capstone
  // readSensor();
  // publishMQTT();

  esp_sleep_enable_timer_wakeup(
    SLEEP_DURATION_SEC * uS_TO_S_FACTOR
  );

  Serial.print("Sleeping for ");
  Serial.print(SLEEP_DURATION_SEC);
  Serial.println("s...");
  Serial.flush();

  esp_deep_sleep_start();
}

void loop() {
  // Never reached — deep sleep restarts from setup()
}
  Serial.println("Loop running - OTA v2!");
  delay(5000);
}

#include <WiFi.h>
#include <time.h>
#include "secrets.h"

// NTP settings
const char* NTP_SERVER1   = "pool.ntp.org";
const char* NTP_SERVER2   = "time.google.com";
const long  GMT_OFFSET_SEC = 19800;  // IST = UTC+5:30 = 5*3600+30*60
const int   DAYLIGHT_OFFSET_SEC = 0; // India has no daylight saving

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("========================");
  Serial.println("  Day 04 - NTP Time Sync");
  Serial.println("========================");

  // Connect WiFi first
  connectWiFi();

  // Sync NTP
  syncNTP();
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 15000) {
      Serial.println("\nWiFi failed!");
      return;
    }
  }

  Serial.println("\nWiFi connected ✓");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void syncNTP() {
  Serial.println("Syncing NTP time...");

  // Configure NTP
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2);

  // Wait for NTP sync (max 10 seconds)
  struct tm timeInfo;
  unsigned long startTime = millis();

  while (!getLocalTime(&timeInfo)) {
    delay(500);
    Serial.print(".");
    if (millis() - startTime > 10000) {
      Serial.println("\nNTP sync failed within 10s!");
      return;
    }
  }

  Serial.println("\nNTP sync successful ✓");
  printTime();
}

void printTime() {
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
    Serial.println("Failed to get time!");
    return;
  }

  Serial.println("========================");
  Serial.print("Date : ");
  Serial.printf("%02d/%02d/%04d\n",
    timeInfo.tm_mday,
    timeInfo.tm_mon + 1,
    timeInfo.tm_year + 1900);

  Serial.print("Time : ");
  Serial.printf("%02d:%02d:%02d\n",
    timeInfo.tm_hour,
    timeInfo.tm_min,
    timeInfo.tm_sec);
  Serial.println("========================");
}

void loop() {
  // Print time every 5 seconds
  printTime();
  delay(5000);
}

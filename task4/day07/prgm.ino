#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "secrets.h"

// ── DHT22 Setup ──────────────────────────────────────────
#define DHTPIN    4
#define DHTTYPE   DHT22
DHT dht(DHTPIN, DHTTYPE);

// ── Config ───────────────────────────────────────────────
const char* TELEMETRY_URL = "https://careers.zelbytes.com/api/iot-lab/v1/telemetry";
const unsigned long INTERVAL_MS = 60000; // 60 seconds per bench policy

unsigned long lastPost = 0;

// ── WiFi Connect ─────────────────────────────────────────
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

// ── POST Telemetry ────────────────────────────────────────
void postTelemetry(float temperature, float humidity) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[ERROR] WiFi disconnected. Reconnecting...");
    connectWiFi();
    return;
  }

  // Build JSON body
  StaticJsonDocument<256> doc;
  doc["device_id"]     = DEVICE_ID;
  doc["temperature_c"] = temperature;       // DHT22 → temperature_c
  doc["humidity_pct"]  = humidity;          // DHT22 → humidity_pct

  String jsonBody;
  serializeJson(doc, jsonBody);

  // HTTP POST
  HTTPClient http;
  http.begin(TELEMETRY_URL);
  http.addHeader("Content-Type",  "application/json");
  http.addHeader("X-Iot-Lab-Key", IOT_LAB_API_KEY);   // Auth header

  int httpCode = http.POST(jsonBody);

  // Log response — mentor will check this
  if (httpCode == 200 || httpCode == 202) {
    Serial.println("[OK] POST success. Code: " + String(httpCode));
    Serial.println("     Payload: " + jsonBody);
  } else if (httpCode == 401) {
    Serial.println("[ERROR] 401 Unauthorized — check your API key in secrets.h");
    // Loop continues — does NOT crash
  } else if (httpCode < 0) {
    Serial.println("[ERROR] HTTP failed: " + http.errorToString(httpCode));
  } else {
    Serial.println("[WARN] Unexpected code: " + String(httpCode));
    Serial.println("       Response: " + http.getString());
  }

  http.end();
}

// ── Setup ─────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();
  connectWiFi();

  Serial.println("=== Zelbytes IoT Lab — Day 7 ===");
  Serial.println("Device ID : " DEVICE_ID);
  Serial.println("Endpoint  : " + String(TELEMETRY_URL));
  Serial.println("Interval  : 60s");
  Serial.println("================================");
}

// ── Loop ──────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  if (now - lastPost >= INTERVAL_MS) {
    lastPost = now;

    float temperature = dht.readTemperature();   // Celsius
    float humidity    = dht.readHumidity();

    // Validate sensor read
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("[ERROR] DHT22 read failed. Check wiring.");
      return;
    }

    Serial.printf("[READ] Temp: %.2f°C  Humidity: %.2f%%\n", temperature, humidity);
    postTelemetry(temperature, humidity);
  }
}

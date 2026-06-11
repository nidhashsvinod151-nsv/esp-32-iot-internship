#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "secrets.h"

// ─── DHT22 Config ───────────────────────────────────────────
#define DHTPIN        4
#define DHTTYPE       DHT22
DHT dht(DHTPIN, DHTTYPE);

// ─── Zelbytes IoT Lab Endpoint ──────────────────────────────
const char* IOT_LAB_URL = "https://careers.zelbytes.com/api/iot-lab/v1/telemetry";

// ─── Median Filter (reuse from Day 5/6) ─────────────────────
#define SAMPLES 5

float medianFilter(float* arr, int n) {
  float sorted[n];
  for (int i = 0; i < n; i++) sorted[i] = arr[i];
  // Bubble sort
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - i - 1; j++)
      if (sorted[j] > sorted[j+1]) {
        float tmp = sorted[j]; sorted[j] = sorted[j+1]; sorted[j+1] = tmp;
      }
  return sorted[n / 2];
}

float readMedianTemp() {
  float readings[SAMPLES];
  for (int i = 0; i < SAMPLES; i++) {
    readings[i] = dht.readTemperature();
    delay(100);
  }
  return medianFilter(readings, SAMPLES);
}

float readMedianHumidity() {
  float readings[SAMPLES];
  for (int i = 0; i < SAMPLES; i++) {
    readings[i] = dht.readHumidity();
    delay(100);
  }
  return medianFilter(readings, SAMPLES);
}

// ─── Post Telemetry to IoT Lab ──────────────────────────────
void postTelemetry(float tempC, float humPct) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Not connected. Skipping POST.");
    return;
  }

  HTTPClient http;
  http.begin(IOT_LAB_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Iot-Lab-Key", ZIL_API_KEY);   // Auth header

  // Build JSON payload — field names required by IoT Lab
  String payload = "{";
  payload += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
  payload += "\"temperature_c\":" + String(tempC, 2) + ",";
  payload += "\"humidity_pct\":" + String(humPct, 2);
  payload += "}";

  Serial.println("[HTTP] Sending payload: " + payload);

  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.printf("[HTTP] Response code: %d\n", httpCode);
    String response = http.getString();
    Serial.println("[HTTP] Response body: " + response);

    if (httpCode == 200 || httpCode == 202) {
      Serial.println("[HTTP] ✓ Telemetry accepted by IoT Lab.");
    } else if (httpCode == 401) {
      Serial.println("[HTTP] ✗ 401 Unauthorized — check your API key in secrets.h");
      // Note: We log and continue — loop does NOT crash
    } else {
      Serial.printf("[HTTP] Unexpected code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] POST failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

// ─── Setup ───────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("\n[Boot] Day 7 — Zelbytes IoT Lab HTTPS Telemetry");

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] Connected. IP: " + WiFi.localIP().toString());
}

// ─── Loop ────────────────────────────────────────────────────
void loop() {
  float temp = readMedianTemp();
  float hum  = readMedianHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("[DHT] Read failed — skipping this cycle.");
  } else {
    Serial.printf("[Sensor] Temp: %.2f °C | Humidity: %.2f %%\n", temp, hum);
    postTelemetry(temp, hum);
  }

  // Post every 60 seconds (polyhouse bench policy)
  delay(60000);
}

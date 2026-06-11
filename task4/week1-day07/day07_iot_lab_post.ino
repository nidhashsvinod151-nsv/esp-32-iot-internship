#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "secrets.h"

#define DHTPIN    4
#define DHTTYPE   DHT22

DHT dht(DHTPIN, DHTTYPE);

// ── SensorBundle struct ───────────────────────────────
struct SensorBundle {
  float temperature;
  float humidity;
  float heatIndex;
  bool  valid;
  unsigned long timestamp;
  int   checksumFails;
};

int checksumFailCount = 0;

// ── Read DHT22 ────────────────────────────────────────
SensorBundle readSensor() {
  SensorBundle bundle;
  bundle.valid         = false;
  bundle.temperature   = 0;
  bundle.humidity      = 0;
  bundle.heatIndex     = 0;
  bundle.timestamp     = millis();
  bundle.checksumFails = checksumFailCount;

  float rawTemp = NAN;
  float rawHum  = NAN;

  for (int attempt = 1; attempt <= 3; attempt++) {
    rawTemp = dht.readTemperature();
    rawHum  = dht.readHumidity();
    if (!isnan(rawTemp) && !isnan(rawHum)) break;
    checksumFailCount++;
    Serial.print("Checksum fail attempt ");
    Serial.println(attempt);
    delay(500);
  }

  if (isnan(rawTemp) || isnan(rawHum)) {
    Serial.println("All 3 attempts failed!");
    return bundle;
  }

  bundle.temperature   = rawTemp;
  bundle.humidity      = rawHum;
  bundle.heatIndex     = dht.computeHeatIndex(rawTemp, rawHum, false);
  bundle.valid         = true;
  bundle.checksumFails = checksumFailCount;
  bundle.timestamp     = millis();
  return bundle;
}

// ── Serialize to JSON ─────────────────────────────────
String buildPayload(SensorBundle b) {
  StaticJsonDocument<256> doc;

  doc["node_id"]        = NODE_ID;
  doc["temp_c"]         = serialized(String(b.temperature, 2));
  doc["humidity_pct"]   = serialized(String(b.humidity, 2));
  doc["heat_index_c"]   = serialized(String(b.heatIndex, 2));
  doc["valid"]          = b.valid;
  doc["timestamp_ms"]   = b.timestamp;
  doc["checksum_fails"] = b.checksumFails;

  String payload;
  serializeJson(doc, payload);
  return payload;
}

// ── Validate payload fields ───────────────────────────
bool validatePayload(SensorBundle b) {
  if (!b.valid) {
    Serial.println("Validation failed: valid = false");
    return false;
  }
  if (b.temperature == 0.0 || isnan(b.temperature)) {
    Serial.println("Validation failed: temp_c invalid");
    return false;
  }
  if (b.humidity == 0.0 || isnan(b.humidity)) {
    Serial.println("Validation failed: humidity invalid");
    return false;
  }
  if (b.temperature < -40 || b.temperature > 80) {
    Serial.println("Validation failed: temp out of range");
    return false;
  }
  if (b.humidity < 0 || b.humidity > 100) {
    Serial.println("Validation failed: humidity out of range");
    return false;
  }
  return true;
}

// ── POST to Zelbytes IoT Lab ──────────────────────────
void postToIoTLab(String payload) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected — skipping POST");
    return;
  }

  HTTPClient http;
  http.begin(IOT_LAB_URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", API_KEY);

  Serial.println("Posting payload:");
  Serial.println(payload);
  Serial.print("Payload size: ");
  Serial.print(payload.length());
  Serial.println(" bytes");

  int httpCode = http.POST(payload);

  Serial.print("HTTP Response: ");
  Serial.println(httpCode);

  if (httpCode == 200 || httpCode == 202) {
    Serial.println("POST successful ✓");
    Serial.println(http.getString());
  } else if (httpCode == 401) {
    Serial.println("401 Unauthorized — invalid API key!");
    Serial.println("Loop continues without crashing ✓");
  } else if (httpCode == 400) {
    Serial.println("400 Bad Request — check payload fields");
    Serial.println(http.getString());
  } else {
    Serial.print("Unexpected error: ");
    Serial.println(httpCode);
  }

  http.end();
}

// ── Deserialize test ──────────────────────────────────
void roundTripTest(String payload) {
  Serial.println("--- Round Trip Test ---");
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("Deserialize failed: ");
    Serial.println(error.c_str());
    return;
  }

  Serial.print("node_id      : "); Serial.println((int)doc["node_id"]);
  Serial.print("temp_c       : "); Serial.println((float)doc["temp_c"]);
  Serial.print("humidity_pct : "); Serial.println((float)doc["humidity_pct"]);
  Serial.print("heat_index_c : "); Serial.println((float)doc["heat_index_c"]);
  Serial.print("valid        : "); Serial.println((bool)doc["valid"] ? "true" : "false");
  Serial.println("Round trip OK ✓");
  Serial.println("-----------------------");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("================================");
  Serial.println("  Day 07 - Zelbytes IoT Lab");
  Serial.println("================================");

  // Connect WiFi
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

  // Init DHT
  dht.begin();
  delay(2000);
}

void loop() {
  // Read sensor
  SensorBundle data = readSensor();

  // Build JSON payload
  String payload = buildPayload(data);

  // Round trip serialize/deserialize test
  roundTripTest(payload);

  // Validate before sending
  if (!validatePayload(data)) {
    Serial.println("Invalid payload — skipping upload");
    delay(10000);
    return;
  }

  // POST to IoT Lab
  postToIoTLab(payload);

  // Wait 10 seconds before next reading
  delay(10000);
}

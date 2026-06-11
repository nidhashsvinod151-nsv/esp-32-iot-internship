#include <Arduino.h>
#include <ArduinoJson.h>

#define SCHEMA_VERSION 1
#define MAX_PAYLOAD 512

bool buildBundle(JsonDocument &doc,
                 const char* node_id, unsigned long ts,
                 float temp_c, float hum, bool tv, const char* fw) {
  if (!node_id || strlen(node_id) == 0) { Serial.println("[ERR] node_id missing"); return false; }
  if (!fw      || strlen(fw)      == 0) { Serial.println("[ERR] fw_version missing"); return false; }
  if (ts == 0)                          { Serial.println("[ERR] timestamp invalid"); return false; }
  doc["schema_version"] = SCHEMA_VERSION;
  doc["node_id"]        = node_id;
  doc["ts"]             = ts;
  doc["temp_c"]         = round(temp_c * 10) / 10.0;
  doc["humidity_pct"]   = round(hum * 10) / 10.0;
  doc["time_valid"]     = tv;
  doc["fw_version"]     = fw;
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Day 8: JSON Payload Design ===\n");

  // --- Valid bundle test ---
  JsonDocument doc;
  bool ok = buildBundle(doc, "esp32-001", 1718092800UL, 28.5, 65.2, true, "1.0.0");
  if (ok) {
    char buf[MAX_PAYLOAD];
    size_t len = serializeJson(doc, buf);
    Serial.println("-- Serialized JSON --");
    Serial.println(buf);
    Serial.print("Size: "); Serial.print(len); Serial.println(" bytes");
    Serial.println(len < MAX_PAYLOAD ? "[OK] Under 512 bytes" : "[WARN] Too large!");

    // Round-trip test
    Serial.println("\n-- Round-Trip Test --");
    JsonDocument parsed;
    auto err = deserializeJson(parsed, buf);
    if (err) { Serial.println("[FAIL] Deserialize error"); }
    else {
      Serial.print("node_id: ");      Serial.println(parsed["node_id"].as<const char*>());
      Serial.print("temp_c: ");       Serial.println(parsed["temp_c"].as<float>(), 1);
      Serial.print("humidity_pct: "); Serial.println(parsed["humidity_pct"].as<float>(), 1);
      Serial.print("time_valid: ");   Serial.println(parsed["time_valid"].as<bool>() ? "true" : "false");
      Serial.println("[OK] Round-trip passed");
    }
  }

  // --- Invalid bundle test ---
  Serial.println("\n-- Invalid Bundle Test --");
  JsonDocument doc2;
  bool ok2 = buildBundle(doc2, "", 1718092800UL, 28.5, 65.2, true, "1.0.0");
  if (!ok2) Serial.println("[OK] Empty node_id correctly rejected");
}

void loop() {}

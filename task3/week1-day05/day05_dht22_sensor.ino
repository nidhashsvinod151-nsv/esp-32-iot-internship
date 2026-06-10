#include <DHT.h>

#define DHTPIN    4
#define DHTTYPE   DHT22

DHT dht(DHTPIN, DHTTYPE);

// ── SensorBundle struct ───────────────────────────────
// Field names match future JSON keys for Zelbytes IoT Lab
// Used on Day 7 (lab dashboard) and Day 12 (MQTT publish)
struct SensorBundle {
  float temperature;       // JSON: "temp_c"
  float humidity;          // JSON: "humidity_pct"
  float heatIndex;         // JSON: "heat_index_c"
  bool  valid;             // JSON: "valid"
  unsigned long timestamp; // JSON: "timestamp_ms"
  int   checksumFails;     // JSON: "checksum_fails"
};

// ── Median filter (last 3 good samples) ──────────────
#define FILTER_SIZE 3
float tempBuffer[FILTER_SIZE] = {0};
float humBuffer[FILTER_SIZE]  = {0};
int   bufferIndex       = 0;
bool  bufferFull        = false;
int   checksumFailCount = 0;

// Sort helper for median
float getMedian(float arr[], int size) {
  float sorted[FILTER_SIZE];
  memcpy(sorted, arr, size * sizeof(float));
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (sorted[j] > sorted[j + 1]) {
        float tmp     = sorted[j];
        sorted[j]     = sorted[j + 1];
        sorted[j + 1] = tmp;
      }
    }
  }
  return sorted[size / 2];
}

// ── Read with retry (up to 3 attempts per cycle) ─────
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

  // Retry up to 3 times per cycle
  for (int attempt = 1; attempt <= 3; attempt++) {
    rawTemp = dht.readTemperature();
    rawHum  = dht.readHumidity();

    if (!isnan(rawTemp) && !isnan(rawHum)) {
      break; // Good reading — exit retry loop
    }

    checksumFailCount++;
    bundle.checksumFails = checksumFailCount;

    Serial.print("⚠ Checksum failure #");
    Serial.print(checksumFailCount);
    Serial.print(" — attempt ");
    Serial.print(attempt);
    Serial.println("/3 (no watchdog reset)");

    delay(500);
  }

  // All 3 attempts failed
  if (isnan(rawTemp) || isnan(rawHum)) {
    Serial.println("✗ All 3 attempts failed — skipping cycle");
    return bundle;
  }

  // Add to median filter buffer
  tempBuffer[bufferIndex] = rawTemp;
  humBuffer[bufferIndex]  = rawHum;
  bufferIndex = (bufferIndex + 1) % FILTER_SIZE;
  if (bufferIndex == 0) bufferFull = true;

  int sampleCount = bufferFull ? FILTER_SIZE : bufferIndex;

  // Apply median filter
  bundle.temperature = getMedian(tempBuffer, sampleCount);
  bundle.humidity    = getMedian(humBuffer,  sampleCount);
  bundle.heatIndex   = dht.computeHeatIndex(
                         bundle.temperature,
                         bundle.humidity,
                         false); // false = Celsius
  bundle.valid     = true;
  bundle.timestamp = millis();

  return bundle;
}

// ── Print in structured format matching JSON fields ───
void printBundle(SensorBundle b) {
  Serial.println("----------------------------");

  if (!b.valid) {
    Serial.println("Reading       : INVALID");
    Serial.print  ("checksum_fails: ");
    Serial.println(b.checksumFails);
    Serial.println("----------------------------");
    return;
  }

  Serial.print("temp_c         : ");
  Serial.print(b.temperature, 2);
  Serial.println(" °C");

  Serial.print("humidity_pct   : ");
  Serial.print(b.humidity, 2);
  Serial.println(" %");

  Serial.print("heat_index_c   : ");
  Serial.print(b.heatIndex, 2);
  Serial.println(" °C");

  Serial.print("valid          : ");
  Serial.println(b.valid ? "true" : "false");

  Serial.print("timestamp_ms   : ");
  Serial.println(b.timestamp);

  Serial.print("checksum_fails : ");
  Serial.println(b.checksumFails);

  Serial.println("----------------------------");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("============================");
  Serial.println("  Day 05 - DHT22 Deep Dive");
  Serial.println("  Zelbytes IoT Lab");
  Serial.println("============================");

  dht.begin();

  Serial.println("Warming up sensor (2s)...");
  delay(2000);
  Serial.println("Ready.");
}

void loop() {
  SensorBundle data = readSensor();
  printBundle(data);
  delay(2000); // DHT22 minimum sample interval
}

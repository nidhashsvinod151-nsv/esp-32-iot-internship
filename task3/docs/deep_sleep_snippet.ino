// ── Deep Sleep Wake Timer Snippet ────────────────────
// Saved for capstone integration — Zelbytes IoT Lab
// ESP32 deep sleep reduces current from 240mA to 0.01mA

#define uS_TO_S_FACTOR 1000000ULL
#define SLEEP_DURATION_SEC 30

// Survives deep sleep — stored in RTC memory
RTC_DATA_ATTR int bootCount = 0;

void printWakeupReason() {
  esp_sleep_wakeup_cause_t wakeup = esp_sleep_get_wakeup_cause();
  switch (wakeup) {
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Wakeup: timer ✓");
      break;
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("Wakeup: external GPIO");
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

  // ── Do sensor read and MQTT publish here ──
  // readSensor();
  // publishMQTT();
  // ──────────────────────────────────────────

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

/*
  CAPSTONE NOTES:
  ──────────────────────────────────────────────────
  1. Replace SLEEP_DURATION_SEC with sample interval
  2. Read DHT22 sensor in setup() before sleep
  3. Publish MQTT in setup() before sleep
  4. Use RTC_DATA_ATTR for variables across sleep

  Power comparison:
  Active WiFi  → 240 mA
  Modem sleep  → 20 mA
  Light sleep  → 2 mA
  Deep sleep   → 0.01 mA  ← use this for battery nodes
*/

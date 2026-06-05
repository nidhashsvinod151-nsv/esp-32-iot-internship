void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=========================");
  Serial.println("  ESP32 Environment Check");
  Serial.println("=========================");

  // SDK version
  Serial.print("SDK Version: ");
  Serial.println(ESP.getSdkVersion());

  // Free heap memory
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  // CPU frequency
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  // Flash size
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");

  // Chip ID
  Serial.print("Chip ID: ");
  Serial.println((uint32_t)ESP.getEfuseMac(), HEX);

  Serial.println("=========================");
  Serial.println("  Day 02 Check Complete!");
  Serial.println("=========================");
}

void loop() {
  // Nothing needed here
}

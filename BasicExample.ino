/*
 * DiceConfigManager - Basic Example
 * 
 * This example demonstrates how to:
 * 1. Initialize the config manager
 * 2. Load configuration from LittleFS
 * 3. Access configuration values
 * 4. Modify configuration
 * 5. Save configuration back to LittleFS
 * 
 * Hardware: ESP32-S3 or any ESP32 with LittleFS support
 */

#include <DiceConfigManager.h>

// Create config manager instance
DiceConfigManager configManager;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== DiceConfigManager Example ===\n");
  
  // Enable verbose output for debugging
  configManager.setVerbose(true);
  
  // Initialize - this will:
  // 1. Mount LittleFS
  // 2. Try to load config.txt
  // 3. Use defaults if file doesn't exist
  if (!configManager.begin("/config.txt", true)) {
    Serial.println("Failed to initialize config manager!");
    return;
  }
  
  Serial.println("\n--- Current Configuration ---");
  configManager.printConfig();
  
  // Example: Access configuration values
  DiceConfig& config = configManager.getConfig();
  Serial.println("\n--- Accessing Individual Values ---");
  Serial.printf("Dice ID: %s\n", config.diceId);
  Serial.printf("RSSI Limit: %d dBm\n", config.rssiLimit);
  Serial.printf("Is SMD: %s\n", config.isSMD ? "Yes" : "No");
  Serial.printf("Deep Sleep Timeout: %u ms\n", config.deepSleepTimeout);
  
  // Example: Modify configuration
  Serial.println("\n--- Modifying Configuration ---");
  configManager.setDiceId("ESP32-001");
  config.rssiLimit = -65;
  config.alwaysSeven = false;
  config.deepSleepTimeout = 600000; // 10 minutes
  
  Serial.println("Configuration modified!");
  
  // Validate configuration
  if (configManager.validate()) {
    Serial.println("Configuration is valid");
  } else {
    Serial.println("Configuration validation failed!");
    Serial.println(configManager.getLastError());
  }
  
  // Save modified configuration
  Serial.println("\n--- Saving Configuration ---");
  if (configManager.save()) {
    Serial.println("Configuration saved successfully!");
  } else {
    Serial.println("Failed to save configuration");
    Serial.println(configManager.getLastError());
  }
  
  // Print final configuration
  Serial.println("\n--- Final Configuration ---");
  configManager.printConfig();
  
  Serial.println("\n=== Example Complete ===");
}

void loop() {
  // Nothing to do here
  delay(1000);
}

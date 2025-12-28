/*
 * DiceConfigManager - File Upload Example
 * 
 * This example demonstrates how to handle config file uploads
 * and reload configuration when files change.
 * 
 * This works with file upload systems like:
 * - ESPConnect
 * - AsyncWebServer with file upload
 * - Serial file transfer
 * 
 * Hardware: ESP32-S3 or any ESP32 with LittleFS support
 */

#include <DiceConfigManager.h>

DiceConfigManager configManager;
unsigned long lastCheck = 0;
const unsigned long CHECK_INTERVAL = 5000; // Check every 5 seconds

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== DiceConfigManager File Upload Example ===\n");
  
  // Enable verbose output
  configManager.setVerbose(true);
  
  // Initialize config manager
  if (!configManager.begin()) {
    Serial.println("Failed to initialize!");
    return;
  }
  
  Serial.println("Config manager initialized");
  Serial.println("Upload a new config.txt file and it will be automatically reloaded");
  Serial.println();
  
  configManager.printConfig();
}

void loop() {
  // Periodically check if config file has been updated
  // In a real application, you might trigger this reload after
  // a file upload event from your web server
  
  if (millis() - lastCheck >= CHECK_INTERVAL) {
    lastCheck = millis();
    
    // Check if user wants to reload config via Serial
    if (Serial.available()) {
      char cmd = Serial.read();
      while (Serial.available()) Serial.read(); // Clear buffer
      
      if (cmd == 'r' || cmd == 'R') {
        Serial.println("\n--- Reloading Configuration ---");
        if (configManager.load()) {
          Serial.println("Configuration reloaded successfully!");
          configManager.printConfig();
        } else {
          Serial.println("Failed to reload configuration");
          Serial.println(configManager.getLastError());
        }
      }
      else if (cmd == 's' || cmd == 'S') {
        Serial.println("\n--- Saving Current Configuration ---");
        if (configManager.save()) {
          Serial.println("Configuration saved!");
        } else {
          Serial.println("Save failed!");
        }
      }
      else if (cmd == 'd' || cmd == 'D') {
        Serial.println("\n--- Resetting to Defaults ---");
        configManager.setDefaults();
        configManager.save();
        configManager.printConfig();
      }
      else if (cmd == 'p' || cmd == 'P') {
        Serial.println();
        configManager.printConfig();
      }
      else if (cmd == 'h' || cmd == 'H') {
        printHelp();
      }
    }
  }
}

void printHelp() {
  Serial.println("\n=== Commands ===");
  Serial.println("R - Reload config from file");
  Serial.println("S - Save current config to file");
  Serial.println("D - Reset to defaults and save");
  Serial.println("P - Print current configuration");
  Serial.println("H - Show this help");
  Serial.println("================\n");
}

// Example: Callback function that could be called after file upload
// This would be integrated with your web server's upload handler
void onFileUploaded(const char* filename) {
  Serial.printf("File uploaded: %s\n", filename);
  
  if (strcmp(filename, "/config.txt") == 0) {
    Serial.println("Config file uploaded, reloading...");
    
    if (configManager.load()) {
      Serial.println("New configuration loaded successfully!");
      
      // Validate the new configuration
      if (configManager.validate()) {
        Serial.println("Configuration validated OK");
        configManager.printConfig();
        
        // Here you might want to apply the new settings
        applyConfiguration();
      } else {
        Serial.println("Warning: Uploaded configuration failed validation");
        Serial.println(configManager.getLastError());
      }
    } else {
      Serial.println("Failed to load uploaded configuration");
      Serial.println(configManager.getLastError());
    }
  }
}

// Example function to apply configuration changes to your application
void applyConfiguration() {
  DiceConfig& config = configManager.getConfig();
  
  Serial.println("\n--- Applying Configuration ---");
  
  // Example: Update WiFi settings based on config
  // setupWiFi(config.diceId);
  
  // Example: Update display colors
  // display.setBackgroundColor(config.x_background);
  
  // Example: Update RSSI threshold
  // espnow.setRSSILimit(config.rssiLimit);
  
  Serial.println("Configuration applied!");
}

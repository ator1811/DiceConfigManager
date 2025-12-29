# DiceConfigManager Library

A lightweight, robust configuration management library for ESP32 projects using LittleFS. Designed for managing dice device configurations with support for MAC addresses, display colors, hardware settings, and operational parameters.

## Features

✅ **Human-Readable Config Files** - Easy to edit .txt files with key=value format  
✅ **Type-Safe Parsing** - Handles MAC addresses, booleans, integers, floats  
✅ **Automatic Checksum** - Data integrity validation  
✅ **Default Values** - Safe fallback configuration  
✅ **Save/Load** - Persistent storage using LittleFS  
✅ **Validation** - Built-in configuration validation  
✅ **Verbose Logging** - Optional debug output  
✅ **Error Handling** - Clear error messages  

## Installation

### Arduino IDE

1. Download this library as a ZIP file
2. In Arduino IDE: `Sketch` → `Include Library` → `Add .ZIP Library`
3. Select the downloaded ZIP file

### Manual Installation

1. Copy the `DiceConfigManager` folder to your Arduino `libraries` folder:
   - **Windows**: `Documents/Arduino/libraries/`
   - **Mac**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`

2. Restart Arduino IDE

## Quick Start

```cpp
#include <DiceConfigManager.h>

DiceConfigManager configManager;

void setup() {
  Serial.begin(115200);
  
  // Initialize with auto-detection (finds *_config.txt automatically)
  configManager.begin();
  
  // Access configuration
  DiceConfig& config = configManager.getConfig();
  Serial.printf("Dice ID: %s\n", config.diceId);
  
  // Modify and save
  config.rssiLimit = -65;
  configManager.save();
}
```

### Auto-Detection Feature

Upload config files with descriptive names like:
- `TEST1_config.txt`
- `BART1_config.txt`  
- `ALICE_config.txt`

The library automatically finds and loads files matching the `*_config.txt` pattern.

**Important:** Only ONE `*_config.txt` file should exist on the ESP32 at a time. If multiple files are found, the library will use default values.

## Configuration File Format

Create a file with a descriptive name matching the pattern `SETNAME_config.txt`:
- `TEST1_config.txt`
- `BART1_config.txt`
- `MYDICE_config.txt`

Or use the traditional name: `config.txt`

File contents (key=value format):

```ini
# Dice Configuration File

# Device Identification
diceId=TEST1

# Device MAC Addresses
deviceA_mac=AA:BB:CC:DD:EE:01
deviceB1_mac=AA:BB:CC:DD:EE:02
deviceB2_mac=AA:BB:CC:DD:EE:03

# Display Colors (RGB565)
x_background=63488      # Red
y_background=2016       # Green
z_background=31         # Blue
entang_ab1_color=65535  # White
entang_ab2_color=0      # Black

# RSSI Settings
rssiLimit=-70

# Hardware Configuration
isSMD=false
isNano=false
alwaysSeven=false

# Operational Parameters
randomSwitchPoint=50
tumbleConstant=2.5
deepSleepTimeout=300000

# Checksum (auto-calculated)
checksum=0
```

## API Reference

### Initialization

```cpp
DiceConfigManager configManager;

// Auto-detect config file (searches for *_config.txt)
bool begin();

// Initialize with default path (/config.txt)
bool begin(const char* configPath);

// Initialize with custom path and format option
bool begin(const char* configPath, bool formatOnFail);
```

**Auto-detection behavior:**
- If `configPath` is `nullptr` (or not provided), searches for files matching `*_config.txt`
- If exactly ONE match found, loads that file
- If zero or multiple matches, uses default configuration
- Verbose mode shows which file was detected

### Load/Save

```cpp
// Load from default path
bool load();

// Load from specific file
bool load(const char* filename);

// Save to default path
bool save();

// Save to specific file
bool save(const char* filename);
```

### Configuration Access

```cpp
// Get reference to config struct
DiceConfig& getConfig();

// Set entire configuration
void setConfig(const DiceConfig& newConfig);

// Individual setters
void setDiceId(const char* id);
void setDeviceAMac(const uint8_t* mac);
void setRssiLimit(int8_t limit);
void setIsSMD(bool value);
// ... and more
```

### Utility Functions

```cpp
// Reset to default values
void setDefaults();

// Validate current configuration
bool validate();

// Print configuration to Serial
void printConfig();

// Get last error message
const char* getLastError();

// Enable/disable debug output
void setVerbose(bool enabled);

// Get currently loaded config file path
const char* getConfigPath();

// Convert MAC to string
String macToString(const uint8_t* mac);
```

## Configuration Structure

```cpp
struct DiceConfig {
  char diceId[16];              // Device identifier
  uint8_t deviceA_mac[6];       // MAC addresses
  uint8_t deviceB1_mac[6];
  uint8_t deviceB2_mac[6];
  uint16_t x_background;        // RGB565 colors
  uint16_t y_background;
  uint16_t z_background;
  uint16_t entang_ab1_color;
  uint16_t entang_ab2_color;
  int8_t rssiLimit;             // RSSI threshold
  bool isSMD;                   // Hardware flags
  bool isNano;
  bool alwaysSeven;
  uint8_t randomSwitchPoint;    // 0-100
  float tumbleConstant;         // Tumble detection
  uint32_t deepSleepTimeout;    // Milliseconds
  uint8_t checksum;             // Data integrity
};
```

## File Upload Integration

### With ESPConnect

**Users upload files with descriptive names:**
- `TEST1_config.txt` for TEST1 dice set
- `BART1_config.txt` for BART1 dice set
- Only upload ONE config file at a time per ESP32

```cpp
#include <DiceConfigManager.h>
// ESPConnect handles file uploads to LittleFS automatically

DiceConfigManager configManager;

void setup() {
  // Auto-detects *_config.txt files
  configManager.begin();
  Serial.printf("Loaded: %s\n", configManager.getConfigPath());
  // ... setup ESPConnect
}

void onConfigUploaded() {
  // Reload after file upload
  if (configManager.load()) {
    Serial.printf("Config reloaded from: %s\n", configManager.getConfigPath());
    configManager.printConfig();
  }
}
```

### With AsyncWebServer

```cpp
server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
  request->send(200);
}, [](AsyncWebServerRequest *request, String filename, size_t index, 
      uint8_t *data, size_t len, bool final) {
  if (final && filename.endsWith("_config.txt")) {
    configManager.load();  // Auto-detects and reloads
  }
});
```

## Default Values

The library provides sensible defaults if no config file exists:

- **diceId**: "DEFAULT"
- **MAC addresses**: 00:00:00:00:00:00
- **Colors**: Red, Green, Blue, White, Black
- **rssiLimit**: -70 dBm
- **Hardware flags**: All false
- **randomSwitchPoint**: 50%
- **tumbleConstant**: 2.5
- **deepSleepTimeout**: 300000 ms (5 minutes)

## Partition Requirements

Ensure your ESP32 partition table includes space for LittleFS:

```csv
spiffs, data, spiffs, 0x810000, 0x100000,
```

For ESP32-S3 with 16MB flash, see the included partition table example.

## Error Handling

```cpp
if (!configManager.load()) {
  Serial.println(configManager.getLastError());
  // Handle error - fall back to defaults
  configManager.setDefaults();
}
```

## Examples

See the `examples` folder for:
- **BasicExample**: Simple load/save/modify workflow
- **FileUploadExample**: Integration with file upload systems

## Troubleshooting

**Config file not loading**
- Check LittleFS is properly mounted
- Verify file path is correct (starts with `/`)
- Enable verbose mode: `configManager.setVerbose(true)`

**Checksum validation failed**
- File may be corrupted
- Set `checksum=0` in config file to disable validation
- Re-save using `configManager.save()` to recalculate

**Upload not working**
- Ensure partition table includes LittleFS/SPIFFS space
- Check file is actually being written to LittleFS
- Use LittleFS.exists("/config.txt") to verify

## License

MIT License - See LICENSE file for details

## Contributing

Contributions welcome! Please submit pull requests or open issues on GitHub.

## Credits

Created for ESP32 Dice project  
Compatible with ESP32, ESP32-S2, ESP32-S3, ESP32-C3

## Version History

- **1.1.0** - Auto-detection feature
  - Automatic detection of `*_config.txt` files
  - Support for descriptive config filenames (e.g., `TEST1_config.txt`)
  - Added `getConfigPath()` method
  - Enhanced error handling for multiple config files
  - Updated all documentation and examples

- **1.0.0** - Initial release
  - Basic load/save functionality
  - Checksum validation
  - Default configuration
  - Verbose logging

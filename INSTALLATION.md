# DiceConfigManager - Installation & Usage Guide

## 📁 Library Structure

```
DiceConfigManager/
├── DiceConfigManager.h          # Header file
├── DiceConfigManager.cpp        # Implementation
├── keywords.txt                 # Arduino IDE syntax highlighting
├── library.properties           # Library metadata
├── README.md                    # Documentation
├── examples/
│   ├── BasicExample/
│   │   └── BasicExample.ino
│   └── FileUploadExample/
│       └── FileUploadExample.ino
└── extras/
    └── config.txt               # Sample configuration file
```

## 🚀 Installation Steps

### Method 1: Manual Installation (Recommended)

1. **Create the library folder:**
   ```
   Navigate to your Arduino libraries folder:
   - Windows: Documents/Arduino/libraries/
   - Mac: ~/Documents/Arduino/libraries/
   - Linux: ~/Arduino/libraries/
   ```

2. **Create folder structure:**
   ```
   DiceConfigManager/
   ├── DiceConfigManager.h
   ├── DiceConfigManager.cpp
   ├── keywords.txt
   ├── library.properties
   ├── README.md
   └── examples/
       ├── BasicExample/
       │   └── BasicExample.ino
       └── FileUploadExample/
           └── FileUploadExample.ino
   ```

3. **Copy files:**
   - Copy `DiceConfigManager.h` and `DiceConfigManager.cpp` to the library folder
   - Copy `keywords.txt` and `library.properties` to the library folder
   - Create `examples` folder and copy example sketches (each in its own subfolder)

4. **Restart Arduino IDE**

### Method 2: ZIP Installation

1. Create a ZIP file with the folder structure above
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. Select your ZIP file
4. Restart Arduino IDE

## 📝 First Time Setup

### 1. Upload Sample Config File

**Important:** Name your config file using the pattern: `SETNAME_config.txt`
- Examples: `TEST1_config.txt`, `BART1_config.txt`, `ALICE_config.txt`
- Or use traditional name: `config.txt`
- Only ONE config file should exist on ESP32 at a time

You have three options:

#### Option A: Using ESPConnect (Recommended)
1. Rename your config file to match your set (e.g., `TEST1_config.txt`)
2. Include ESPConnect in your sketch
3. Connect to the device's WiFi AP
4. Navigate to https://thelastoutpostworkshop.github.io/ESPConnect/
5. Upload your `SETNAME_config.txt` file

#### Option B: Using Arduino IDE Serial Upload
```cpp
void uploadConfigViaSerial() {
  // Use your set name in the filename
  File file = LittleFS.open("/TEST1_config.txt", "w");
  if (file) {
    file.println("diceId=TEST1");
    file.println("deviceA_mac=AA:BB:CC:DD:EE:01");
    // ... add more lines
    file.close();
  }
}
```

#### Option C: Using LittleFS Upload Tool
1. Install "ESP32 LittleFS Data Upload" plugin
2. Create a `data` folder in your sketch folder
3. Put your `SETNAME_config.txt` in the data folder
4. Tools → ESP32 Sketch Data Upload

### 2. Basic Example

```cpp
#include <DiceConfigManager.h>

DiceConfigManager config;

void setup() {
  Serial.begin(115200);
  
  // Initialize (auto-detects *_config.txt files)
  if (!config.begin()) {
    Serial.println("Init failed!");
    return;
  }
  
  // Show which config file was loaded
  Serial.printf("Loaded: %s\n", config.getConfigPath());
  
  // Use the configuration
  DiceConfig& cfg = config.getConfig();
  Serial.printf("Dice ID: %s\n", cfg.diceId);
  Serial.printf("RSSI Limit: %d\n", cfg.rssiLimit);
}
```

**The library automatically:**
- Searches for files matching `*_config.txt` pattern
- Loads the file if exactly one is found
- Uses defaults if zero or multiple files found

## 🔧 Configuration File Setup

### Step 1: Get Your MAC Addresses

Run this sketch to find your device MAC addresses:

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  
  Serial.printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void loop() {}
```

### Step 2: Edit and Rename Config File

1. Open the sample `config.txt`
2. **Rename it** to match your set: `SETNAME_config.txt` (e.g., `TEST1_config.txt`)
3. Replace MAC addresses with your actual device MACs
4. Set your desired dice ID (should match the set name for clarity)
5. Adjust colors if needed (RGB565 format)
6. Set RSSI limit based on your testing
7. Configure hardware flags (isSMD, isNano)

### Step 3: Upload to ESP32

Use ESPConnect or LittleFS upload tool to put the file on your device.

## 🎨 RGB565 Color Reference

Quick color calculator:
```
R (5 bits): 0-31    → shift left 11
G (6 bits): 0-63    → shift left 5
B (5 bits): 0-31    → no shift

Color = (R << 11) | (G << 5) | B
```

Common colors:
```ini
# Pure colors
x_background=63488    # Red (31,0,0)
y_background=2016     # Green (0,63,0)
z_background=31       # Blue (0,0,31)

# Mixed colors
entang_ab1_color=65535  # White (31,63,31)
entang_ab2_color=0      # Black (0,0,0)

# Custom colors
orange=64512            # Orange (31,32,0)
purple=48095            # Purple (23,0,31)
cyan=2047              # Cyan (0,63,31)
```

## 🔄 Integration with ESPConnect

### Complete Example

```cpp
#include <DiceConfigManager.h>
#include <ESPConnect.h>

DiceConfigManager config;
ESPConnect espConnect;

void onFileUploaded() {
  Serial.println("File uploaded, reloading config...");
  if (config.load()) {
    config.printConfig();
    applyNewSettings();
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize config
  config.begin();
  
  // Setup ESPConnect
  espConnect.begin("DiceConfig");
  espConnect.onFileUpload(onFileUploaded);
  
  // Apply initial settings
  applyNewSettings();
}

void applyNewSettings() {
  DiceConfig& cfg = config.getConfig();
  // Use cfg values in your application
}
```

## 🐛 Debugging Tips

### Enable Verbose Output
```cpp
config.setVerbose(true);
config.load();  // Will print detailed parsing info
```

### Check File Exists
```cpp
if (LittleFS.exists("/config.txt")) {
  Serial.println("Config file found!");
} else {
  Serial.println("Config file missing!");
}
```

### List Files
```cpp
File root = LittleFS.open("/");
File file = root.openNextFile();
while (file) {
  Serial.println(file.name());
  file = root.openNextFile();
}
```

### Validate After Loading
```cpp
if (config.load()) {
  if (!config.validate()) {
    Serial.println("Config loaded but validation failed!");
    Serial.println(config.getLastError());
  }
}
```

## 📊 Partition Table Reference

For ESP32-S3 16MB, add to your sketch folder as `partitions.csv`:

```csv
# Name,   Type, SubType, Offset,  Size,    Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x400000,
app1,     app,  ota_1,   0x410000,0x400000,
spiffs,   data, spiffs,  0x810000,0x100000,
```

Select in Arduino IDE:
- Tools → Partition Scheme → Custom
- Or use built-in "16M Flash (3MB APP/9.9MB FATFS)"

## 🎯 Best Practices

1. **Always validate** after loading user-uploaded configs
2. **Use defaults** as fallback if load fails
3. **Enable verbose mode** during development
4. **Calculate checksum** before saving (`save()` does this automatically)
5. **Test config changes** before deploying
6. **Keep backups** of working configs
7. **Version your configs** using comments in the file

## ❓ FAQ

**Q: Can I add custom fields?**
A: Yes! Modify the `DiceConfig` struct and add parsing in `load()` and saving in `save()`.

**Q: What if I don't need checksums?**
A: Set `checksum=0` in your config file to disable validation.

**Q: Can I use SPIFFS instead of LittleFS?**
A: The library uses LittleFS, but you can modify it to use SPIFFS if needed.

**Q: How do I reset to defaults?**
A: Call `config.setDefaults()` then `config.save()`.

**Q: File upload not working?**
A: Check partition table has space for LittleFS and verify file is written to `/config.txt`.

## 📞 Support

- Check examples folder for reference implementations
- Enable verbose mode for detailed error messages
- Use `getLastError()` to see what went wrong

Happy configuring! 🎲

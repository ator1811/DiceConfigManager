# 🚀 Quick Start Guide - DiceConfigManager

## ⚡ 5-Minute Setup

### 1. Install the Library (30 seconds)

**Option A - Copy to Arduino Libraries:**
```
1. Copy the entire DiceConfigManager folder to:
   - Windows: Documents/Arduino/libraries/
   - Mac: ~/Documents/Arduino/libraries/
   - Linux: ~/Arduino/libraries/

2. Restart Arduino IDE

3. Verify: File → Examples → DiceConfigManager → BasicExample
```

**Option B - ZIP Install:**
```
1. Compress DiceConfigManager folder to a .zip
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. Select your ZIP file
4. Restart Arduino IDE
```

### 2. Test the Library (2 minutes)

**Open the Basic Example:**
```
File → Examples → DiceConfigManager → BasicExample
```

**Upload to your ESP32-S3:**
```
1. Connect your ESP32-S3
2. Select board: Tools → Board → ESP32 Arduino → ESP32S3 Dev Module
3. Select partition: Tools → Partition Scheme → 16M Flash (3MB APP/9.9MB FATFS)
4. Click Upload
5. Open Serial Monitor (115200 baud)
```

**You should see:**
```
=== DiceConfigManager Example ===
LittleFS mounted successfully
No config file found, using defaults

--- Current Configuration ---
Dice ID: DEFAULT
RSSI Limit: -70 dBm
...
```

### 3. Upload Your Config File (2 minutes)

**Edit config.txt:**
```ini
diceId=MYDICE1
deviceA_mac=AA:BB:CC:DD:EE:01  # Replace with your actual MAC
rssiLimit=-70
isSMD=false
```

**Upload via ESPConnect:**
```
1. Use ESPConnect in your sketch to create WiFi AP
2. Connect to the AP
3. Visit: https://thelastoutpostworkshop.github.io/ESPConnect/
4. Upload your config.txt file
```

**Or upload via LittleFS tool:**
```
1. Create "data" folder in your sketch directory
2. Put config.txt in data folder
3. Tools → ESP32 Sketch Data Upload
```

### 4. Use in Your Project (1 minute)

**Minimal working example:**

```cpp
#include <DiceConfigManager.h>

DiceConfigManager config;

void setup() {
  Serial.begin(115200);
  
  // Auto-loads from /config.txt
  config.begin();
  
  // Access values
  DiceConfig& cfg = config.getConfig();
  Serial.printf("ID: %s\n", cfg.diceId);
  Serial.printf("RSSI: %d\n", cfg.rssiLimit);
  
  // Modify
  cfg.rssiLimit = -65;
  
  // Save
  config.save();
}

void loop() {
  // Your code here
}
```

## 📝 Common Tasks

### Get MAC Address
```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}
```

### Reload Config After Upload
```cpp
void onConfigUploaded() {
  config.load();
  config.printConfig();
}
```

### Reset to Defaults
```cpp
config.setDefaults();
config.save();
```

### Debug Config Issues
```cpp
config.setVerbose(true);
if (!config.load()) {
  Serial.println(config.getLastError());
}
```

## 🎯 Next Steps

1. ✅ Library installed and tested
2. ✅ Config file uploaded
3. ✅ Basic usage working

**Now:**
- Check out FileUploadExample for web-based config updates
- Customize the DiceConfig struct for your needs
- Integrate with ESPConnect for OTA config management
- Read README.md for full API documentation

## ⚠️ Troubleshooting

**"LittleFS Mount Failed"**
→ Check partition table has LittleFS/SPIFFS space

**"Config file not found"**
→ Use config.setDefaults() as fallback
→ Upload config.txt using LittleFS tool

**"Checksum validation failed"**
→ Set checksum=0 in config.txt
→ Or re-save using config.save()

**Examples not showing**
→ Restart Arduino IDE
→ Check library folder structure

## 📚 Files Included

```
DiceConfigManager/
├── DiceConfigManager.h          ← Header file
├── DiceConfigManager.cpp        ← Implementation
├── keywords.txt                 ← Syntax highlighting
├── library.properties           ← Library info
├── README.md                    ← Full documentation
├── INSTALLATION.md              ← Detailed setup guide
├── config.txt                   ← Sample config template
├── QUICKSTART.md               ← This file!
└── examples/
    ├── BasicExample/            ← Start here!
    │   └── BasicExample.ino
    └── FileUploadExample/       ← Advanced usage
        └── FileUploadExample.ino
```

## 💡 Pro Tips

1. Always call `begin()` before accessing config
2. Use `validate()` after loading user-uploaded configs
3. Enable verbose mode during development
4. Keep a backup of your working config.txt
5. Use the examples as templates for your project

**Happy coding! 🎲**

Need help? Check README.md or INSTALLATION.md for detailed guides.

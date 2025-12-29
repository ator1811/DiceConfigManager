# DiceConfigManager v1.1.0 - Auto-Detection Update

## What's New

### ✨ Automatic Config File Detection

The library now automatically detects and loads configuration files matching the `*_config.txt` pattern!

**Before (v1.0.0):**
```cpp
config.begin("/config.txt");  // Always needed explicit path
```

**Now (v1.1.0):**
```cpp
config.begin();  // Auto-detects TEST1_config.txt, BART1_config.txt, etc.
```

## Key Changes

### 1. **Descriptive Filenames**
Users can now name their config files with meaningful names:
- `TEST1_config.txt` - for TEST1 dice set
- `BART1_config.txt` - for BART1 dice set  
- `ALICE_config.txt` - for ALICE dice set
- Or traditional `config.txt` still works

### 2. **Auto-Detection Logic**
When `begin()` is called without a path:
- ✅ Searches for files matching `*_config.txt` pattern
- ✅ If **exactly ONE** file found → loads it automatically
- ✅ If **zero** files found → uses default configuration
- ⚠️ If **multiple** files found → uses default configuration (prevents ambiguity)

### 3. **New API Method**
```cpp
const char* getConfigPath();
```
Returns the currently loaded config file path. Useful for debugging and logging.

### 4. **Enhanced Error Handling**
- Clear error messages when multiple config files detected
- Verbose mode shows which file was auto-detected
- Better logging for troubleshooting

### 5. **Code Cleanup**
- Removed unused MAC address-based detection functions
- Removed WiFi.h dependency (no longer needed)
- Cleaner, more focused codebase
- Reduced from 696 to 562 lines in main implementation

## Usage Example

### Simple Auto-Detection
```cpp
#include <DiceConfigManager.h>

DiceConfigManager config;

void setup() {
  Serial.begin(115200);
  config.setVerbose(true);
  
  // Auto-detects *_config.txt files
  config.begin();
  
  // See which file was loaded
  Serial.printf("Loaded: %s\n", config.getConfigPath());
  
  // Use configuration
  DiceConfig& cfg = config.getConfig();
  Serial.printf("Dice ID: %s\n", cfg.diceId);
}
```

### Output Example
```
LittleFS mounted successfully
No config path specified, searching for *_config.txt...
Found config file: /TEST1_config.txt
Auto-detected config file: /TEST1_config.txt
Config loaded successfully
Loaded: /TEST1_config.txt
Dice ID: TEST1
```

## User Workflow

### For Users Managing Multiple Sets

**Old Workflow:**
1. Edit config.txt
2. Upload to ESP32-A
3. Edit config.txt again
4. Upload to ESP32-B (overwrites!)
5. Confusion: which config is on which device?

**New Workflow:**
1. Create `TEST1_config.txt` on PC
2. Upload to TEST1 ESP32s (keeps descriptive name!)
3. Create `BART1_config.txt` on PC
4. Upload to BART1 ESP32s
5. Clear which set each device belongs to ✨

### Important Rules

**✅ DO:**
- Use descriptive names: `SETNAME_config.txt`
- Keep only ONE config file per ESP32
- Match diceId inside file with filename for clarity

**❌ DON'T:**
- Upload multiple `*_config.txt` files to same ESP32
- Mix naming conventions on same device

## Migration from v1.0.0

### No Code Changes Required!

Your existing code will work without modification:

```cpp
// This still works exactly as before
config.begin("/config.txt");
```

### Optional: Enable Auto-Detection

To use the new feature, simply remove the path:

```cpp
// Before
config.begin("/config.txt");

// After (with renamed file to TEST1_config.txt)
config.begin();
```

## Technical Details

### File Search Implementation
- Searches root directory of LittleFS
- Uses pattern matching on filenames
- Case-sensitive: looks for `*_config.txt` (lowercase)
- Stops at first directory level (doesn't recurse)

### Error Conditions
- **No files found**: Uses defaults, no error
- **Multiple files found**: Uses defaults, logs error
- **LittleFS mount fails**: Returns false, error logged

### Performance
- Minimal overhead: file search only runs once during `begin()`
- No impact on load/save operations
- No additional memory usage

## Backward Compatibility

**100% backward compatible!**
- Old code with explicit paths works unchanged
- Old config.txt files still supported
- No breaking changes to API
- Existing functionality preserved

## Files Updated

### Core Library
- ✏️ `DiceConfigManager.h` - Added `findConfigFile()` and `getConfigPath()`
- ✏️ `DiceConfigManager.cpp` - Implemented auto-detection logic
- ✏️ `keywords.txt` - Added `getConfigPath` keyword
- ✏️ `library.properties` - Updated version to 1.1.0

### Documentation
- ✏️ `README.md` - Auto-detection feature documented
- ✏️ `QUICKSTART.md` - Updated with new naming pattern
- ✏️ `INSTALLATION.md` - Updated all examples

### Examples
- ✏️ `BasicExample.ino` - Demonstrates auto-detection
- ✏️ `FileUploadExample.ino` - Updated for new pattern

## Testing Checklist

Before deploying v1.1.0, test:

- ✅ Auto-detection with single `*_config.txt` file
- ✅ Fallback to defaults with zero config files
- ✅ Fallback to defaults with multiple config files
- ✅ Backward compatibility with explicit path
- ✅ Traditional `config.txt` still works
- ✅ `getConfigPath()` returns correct path
- ✅ Verbose logging shows detection process
- ✅ File upload and reload works correctly

## Troubleshooting

### "Multiple config files found" Error
**Problem:** You uploaded more than one `*_config.txt` file
**Solution:** Delete extra files, keep only one

```cpp
// Check what files exist
File root = LittleFS.open("/");
File file = root.openNextFile();
while (file) {
  Serial.println(file.name());
  file = root.openNextFile();
}
```

### Auto-detection not finding file
**Problem:** Filename doesn't match pattern
**Solution:** Ensure filename ends with `_config.txt` (lowercase)

Valid: `TEST1_config.txt`, `abc123_config.txt`
Invalid: `TEST1_CONFIG.txt`, `test1-config.txt`, `config_TEST1.txt`

### Want to use specific file despite multiple
**Problem:** Multiple config files on device
**Solution:** Use explicit path instead of auto-detection

```cpp
// Explicitly load specific file
config.begin("/TEST1_config.txt");
```

## Future Enhancements (Not in v1.1.0)

Possible features for future versions:
- Config file selector via web UI
- Support for subdirectories
- Active config file tracking
- Config file validation on upload

## Credits

Feature implemented based on user feedback for managing multiple ESP32 dice sets with different configurations.

---

**Version:** 1.1.0  
**Release Date:** December 2024  
**License:** MIT

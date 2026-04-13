# Fish - Minecraft Auto-Fisher

A DLL injection-based auto-fishing tool for Minecraft (CosmicClient).

## Features

- **Automatic fish detection** - Detects when a fish bites your bobber
- **Auto-reel** - Automatically reels in the fish when detected
- **Auto-recast** - Casts your rod again after catching
- **Bobber tracking** - Tracks your fishing bobber by UUID to avoid false positives
- **Sound-based detection** - Verifies fish bite by checking sound location against bobber position

## Requirements

- Windows 10/11
- Visual Studio 2019 or 2022 with C++ Desktop Development
- CMake 3.20+
- Java Development Kit (JDK) 8 or higher
- Minecraft running with CosmicClient

## Building

### Option 1: Using build script

```batch
build.bat
```

### Option 2: Manual CMake

```batch
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

### Output

After building, you'll find:
- `build/bin/Release/Fish.dll` - The main DLL
- `build/bin/Release/FishInjector.exe` - The injector

## Usage

1. **Start Minecraft** with CosmicClient
2. **Run FishInjector.exe** as Administrator
3. **Hold a fishing rod** and cast it into water
4. The auto-fisher will automatically:
   - Track your bobber
   - Detect when a fish bites
   - Reel in the fish
   - Recast the rod

### Controls

| Key | Action |
|-----|--------|
| `INSERT` | Toggle auto-fishing on/off |
| `HOME` | Manually update bobber position |
| `END` | Eject the DLL |

## Configuration

You can adjust settings in the Fish console:

```cpp
// Delays (in milliseconds)
g_hack->module_handler.get_fishing().set_recast_delay(500);  // Time before recasting
g_hack->module_handler.get_fishing().set_reel_delay(100);    // Time before reeling

// Detection sensitivity
g_hack->module_handler.get_fishing().set_sound_tolerance(0.2);  // How close sound must be to bobber
g_hack->module_handler.get_fishing().set_max_bobber_distance(64.0);  // Max distance for bobber
```

## Project Structure

```
Fish/
├── CMakeLists.txt          # Build configuration
├── build.bat               # Windows build script
├── java/
│   ├── dllmain.cpp         # DLL entry point
│   ├── utils/
│   │   └── logging.h       # Console logging
│   ├── math/
│   │   └── vec.h           # Vector math
│   ├── libraries/
│   │   └── xor/
│   │       └── xorstr.h    # String encryption
│   ├── injector/
│   │   └── injector.cpp    # DLL injector
│   ├── main/
│   │   ├── hack.h          # Main hack class
│   │   └── modules/
│   │       ├── module_handler.h
│   │       ├── fishing/
│   │       │   └── fishing.h    # Auto-fishing module
│   │       └── chams/
│   │           └── chams.h      # Entity chams (placeholder)
│   └── java/
│       ├── java.cpp        # JNI bridge implementation
│       ├── java.h          # JNI bridge header
│       ├── native_bridge.h # Embedded Java bytecode
│       ├── internal/       # Minecraft class wrappers
│       ├── mappings/       # Obfuscation mappings
│       └── redefined_classes/  # Hooked class bytecode
```

## How It Works

1. **DLL Injection**: The injector loads Fish.dll into the Minecraft process
2. **JVM Attachment**: The DLL attaches to the running JVM using JNI
3. **Class Redefinition**: Uses JVMTI to redefine Minecraft classes with hooks
4. **NativeBridge**: A custom Java class that bridges Java events to native C++
5. **Fishing Detection**: When the `onFish` callback is triggered:
   - Verifies the sound is near the cached bobber position
   - Sends a right-click to reel in
   - Waits, then recasts

## Troubleshooting

### "JVM not found"
- Make sure Minecraft is running before injection
- The injector looks for `javaw.exe` by default

### "Failed to open process"
- Run the injector as Administrator
- Disable any anti-cheat software

### "Mapping set failed"
- The mappings are for CosmicClient specifically
- Other clients may need different mappings

## Disclaimer

This tool is for educational purposes only. Using it may violate Minecraft's Terms of Service or server rules. Use at your own risk.

## License

MIT License - See [LICENSE](LICENSE) for details.

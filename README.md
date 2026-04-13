# CosmicClient Auto-Fisher

Automatic fishing bot for Minecraft CosmicClient. Detects fish bites and reels in automatically.

---

## Quick Start

### 1. Download
Download `Fish.dll` from this repo (click the file -> Download)

### 2. Inject
1. Launch **CosmicClient** and join a server
2. Open **Process Hacker** (run as Administrator)
3. Find `javaw.exe` -> Right-click -> **Miscellaneous** -> **Inject DLL**
4. Select `Fish.dll`

### 3. Use
- Equip a **fishing rod** and cast into water
- Press **INSERT** to toggle auto-fishing ON/OFF
- Press **HOME** to see fish count
- Press **END** to eject

---

## How It Works
- Tracks your fishing bobber's Y position
- When the bobber drops (fish bite), it auto-reels
- Automatically recasts after catching

---

## Log File
Check `Desktop\fish_log.txt` for debug info.

---

## Building (Optional)
If you want to build from source:
```powershell
cmake -B build -A x64
cmake --build build --config Release
```
Output: `build\bin\Release\Fish.dll`

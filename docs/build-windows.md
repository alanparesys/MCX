# Windows Build Configuration

This document describes how to build MCX on Windows.

## Prerequisites

- Windows 10 or Windows 11
- Visual Studio 2022 (Community edition is free)
- CMake 3.16 or higher
- Git for Windows

## Installing Dependencies

### Visual Studio 2022

Download from: https://visualstudio.microsoft.com/downloads/

During installation, select:
- **Desktop development with C++**
- Windows 10/11 SDK
- CMake tools for Windows

### CMake

Download from: https://cmake.org/download/

Or install via Chocolatey:
```powershell
choco install cmake
```

### Git

Download from: https://git-scm.com/download/win

## Building MCX

### Option 1: Visual Studio IDE

1. Open Visual Studio 2022
2. Select "Clone a repository"
3. Enter: `https://github.com/alanparesys/MCX.git`
4. Click "Clone"
5. Go to `File → Open → CMake`
6. Select `CMakeLists.txt` from the cloned folder
7. Visual Studio will automatically configure the project
8. Select `x64-Release` from the configuration dropdown
9. Click `Build → Build All`

### Option 2: Command Line (Developer PowerShell)

```powershell
# Open "Developer PowerShell for VS 2022" from Start Menu

# Clone repository
git clone https://github.com/alanparesys/MCX.git
cd MCX

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -S .. -B . -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release --parallel

# Output files will be in Release/ folder
```

### Option 3: MinGW-w64 (Alternative)

```powershell
# Install MinGW-w64 via MSYS2 or Chocolatey
choco install mingw

# Configure
cmake -S .. -B . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --parallel
```

## Output Files

After successful build, you'll find:

```
build/
├── Release/
│   ├── mcx.exe                 # Main server executable
│   ├── mcx_tests.exe           # Test executable
│   └── mcx_core.lib            # Core library
└── modules/
    └── sample_module.dll       # Sample module
```

## Running MCX on Windows

```powershell
# Navigate to build directory
cd build\Release

# Run setup wizard
.\mcx.exe --setup

# Start server
.\mcx.exe --server

# Or run demo
.\mcx.exe --demo
```

## Creating Windows Release Package

```powershell
# Create distribution folder
mkdir mcx-windows-x64
cd mcx-windows-x64

# Copy files
copy ..\build\Release\mcx.exe .
copy ..\build\Release\mcx_tests.exe .
copy ..\build\modules\sample_module.dll .\modules\
copy ..\config.json .

# Create batch file for easy start
@echo @echo off > start-mcx.bat
@echo MCX Server for Windows >> start-mcx.bat
@echo ====================== >> start-mcx.bat
@echo. >> start-mcx.bat
@echo Run setup first if you haven't: >> start-mcx.bat
@echo   .\mcx.exe --setup >> start-mcx.bat
@echo. >> start-mcx.bat
@echo Starting MCX server... >> start-mcx.bat
@echo. >> start-mcx.bat
@echo .\mcx.exe --server >> start-mcx.bat
@echo pause >> start-mcx.bat

# Create archive
Compress-Archive -Path * -DestinationPath ..\mcx-v0.1.0-windows-x64.zip
```

## Windows-Specific Configuration

### Config File (config.json)

```json
{
  "backendEndpoint": "",
  "scriptRoot": "scripts",
  "modulesPath": "modules",
  "maxPlayers": 50,
  "demoMode": false,
  "rcon": {
    "enabled": true,
    "port": 25575,
    "password": "your-secure-password"
  },
  "websocket": {
    "enabled": true,
    "port": 8080,
    "host": "127.0.0.1"
  }
}
```

### Firewall Configuration

Windows Defender Firewall will prompt on first run. Allow access for:
- **mcx.exe** - for ports 25565 (Minecraft) and 8080 (WebSocket)

Or configure manually:
```powershell
# Run as Administrator
New-NetFirewallRule -DisplayName "MCX Server" -Direction Inbound -Program "C:\path\to\mcx.exe" -Action Allow
```

## Troubleshooting

### "CMake not found"
Add CMake to PATH or use "Developer PowerShell for VS 2022"

### "Missing VCRUNTIME140.dll"
Install Visual C++ Redistributable:
https://aka.ms/vs/17/release/vc_redist.x64.exe

### Build errors with Windows SDK
Install Windows 10/11 SDK via Visual Studio Installer

### Winsock errors
The code includes `#pragma comment(lib, "ws2_32.lib")` for automatic linking. If you get linker errors, add `ws2_32.lib` to project linker settings.

## Development on Windows

### Recommended IDEs
- **Visual Studio 2022** (best CMake support)
- **CLion** (JetBrains, cross-platform)
- **VS Code** with C++ extension

### Debugging
In Visual Studio:
1. Set breakpoint in code
2. Select `mcx.exe` as startup item
3. Press F5 to debug

### Testing
```powershell
cd build\Release
.\mcx_tests.exe
```

## Cross-Compilation from Linux

If you need to build Windows binaries from Linux:

```bash
# Install MinGW-w64
sudo apt install mingw-w64

# Configure
cmake -S .. -B . \
  -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64-x86_64.cmake \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --parallel
```

---

For more information, see the main [README.md](../README.md)

# MCX v0.1.0 Windows Release

## 📦 Downloads

| Platform | Architecture | Package | Size |
|----------|-------------|---------|------|
| **Windows** | x64 | [mcx-v0.1.0-windows-x64.zip](https://github.com/alanparesys/MCX/releases/download/v0.1.0/mcx-v0.1.0-windows-x64.zip) | ~2 MB |
| **Linux** | x64 | [mcx-v0.1.0-linux-x64.tar.gz](https://github.com/alanparesys/MCX/releases/download/v0.1.0/mcx-v0.1.0-linux-x64.tar.gz) | 174 KB |

## 🚀 Quick Start (Windows)

### Option 1: Pre-built Binary

1. Download `mcx-v0.1.0-windows-x64.zip`
2. Extract to a folder (e.g., `C:\MCX`)
3. Run `start-mcx.bat` or open PowerShell:

```powershell
cd C:\MCX
.\mcx.exe --setup    # Interactive configuration
.\mcx.exe --server   # Start server
```

### Option 2: Build from Source

**Requirements:**
- Visual Studio 2022 (Desktop C++ workload)
- CMake 3.16+

**Build:**

```powershell
# Open "Developer PowerShell for VS 2022"
git clone https://github.com/alanparesys/MCX.git
cd MCX
.\build-windows.bat
```

Or manually:

```powershell
cd MCX
mkdir build && cd build
cmake -S .. -B . -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release --parallel
```

## 📁 Package Contents (Windows)

```
mcx-v0.1.0-windows-x64/
├── mcx.exe              # Main server executable
├── mcx_tests.exe        # Unit tests
├── config.json          # Default configuration
├── start-mcx.bat        # Easy start script
├── README.txt           # Quick start guide
└── modules/
    └── sample_module.dll # Example module
```

## ⚙️ Windows Configuration

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

### Firewall

Windows will prompt to allow network access. Click "Allow" for:
- Port 25565 (Minecraft Java)
- Port 8080 (WebSocket API)

Or manually configure:
```powershell
# Run as Administrator
New-NetFirewallRule -DisplayName "MCX Server" -Direction Inbound -Program "C:\MCX\mcx.exe" -Action Allow
```

## 🎮 Testing

1. Start MCX: `.\mcx.exe --demo`
2. Open Minecraft Java Edition
3. Add server: `localhost:25565`
4. Join and type in chat:
   - `!hello` - Test response
   - `!scene test` - Test scene change

## ✨ Features

- **30+ Core Systems**: EventBus, Scheduler, Economy, Teleport, etc.
- **Module System**: Extend with DLL modules
- **Lua Scripting**: Optional (requires Lua 5.4 installed separately on Windows)
- **Network**: HTTP client, WebSocket server, RCON
- **Windows Native**: Built with MSVC for optimal performance

## 📚 Documentation

- [Windows Build Guide](docs/build-windows.md)
- [Full Documentation](https://github.com/alanparesys/MCX-docs)
- [Release Notes](RELEASES.md)

## ⚠️ Known Limitations

- Lua runtime requires manual installation of Lua 5.4 on Windows
- Paper/Spigot integration requires separate plugin development
- Some network features may require Administrator privileges

## 🔧 Troubleshooting

### "VCRUNTIME140.dll not found"
Install Visual C++ Redistributable: https://aka.ms/vs/17/release/vc_redist.x64.exe

### "Windows protected your PC"
Click "More info" → "Run anyway". This is normal for unsigned executables.

### Build fails with "CMake not found"
Add CMake to PATH or use "Developer PowerShell for VS 2022" which includes CMake.

---

**Labels**: release/stable, version/v0.1.0, platform/windows
**Full Changelog**: https://github.com/alanparesys/MCX/compare/v0.1.0...HEAD

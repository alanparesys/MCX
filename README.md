# MCX

Minecraft C++ Experience Layer. A server-side scripting platform for Minecraft that enables custom game modes and experiences without client modifications.

## Features

- Event-driven architecture (player join/quit, chat, commands, scene transitions)
- Scriptable runtime with Lua integration planned
- Scene management for distinct game modes (lobby, arena, race, etc.)
- Player registry with automatic join/quit tracking
- Zero-allocation logging hot path
- JSON-based configuration
- Fake event source for local testing without Minecraft server

## Architecture

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   Minecraft │────▶│  MCX Core   │────▶│   Actions   │
│   Events    │     │  (C++20)    │     │  (Log/Exec) │
└─────────────┘     └──────┬──────┘     └─────────────┘
                           │
                    ┌──────┴──────┐
                    │ ScriptRuntime│
                    │ (Hardcoded/ │
                    │  Lua future) │
                    └─────────────┘
```

Core (`src/core/`): Server, Event/Action system, ScriptRuntime interface, SceneManager, PlayerRegistry
Integration (`src/integration/`): Minecraft bridge, fake event source for testing

## C++ Build & Development

### Requirements

- C++20 compatible compiler
- CMake 3.16 or higher

**Supported compilers:**
- GCC 10+
- Clang 12+
- MSVC 2019+

### Dependencies

Header-only dependencies included in `third_party/`:
- nlohmann/json (v3.11.3)

No external package manager required.

### Build Steps (Linux)

```bash
# Configure Release build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --parallel

# Run
cmake --build build --target mcx
```

### Build Steps (Windows)

```powershell
# Configure Release build (Visual Studio)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Release
```

### Debug Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Clean Rebuild

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

### Build Targets

| Target | Description |
|--------|-------------|
| `mcx` | Main server executable |
| `mcx_tests` | Unit test executable |
| `mcx_core` | Static library (core components) |

### Run

```bash
./build/mcx --demo        # Run fake event demo
./build/mcx --version     # Show version
```

### Compile Flags (Applied)

Release builds: `-O2 -DNDEBUG`
Debug builds: `-g -O0`

C++20 features used: `std::variant`, `std::optional`, designated initializers.

## Project Structure

```
include/mcx/          Public headers
src/core/             Server, runtime, events, actions
src/integration/      Minecraft bridge, fake events
tests/                Test binaries
docs/                 Architecture and API docs
third_party/          External dependencies (nlohmann/json)
```

## Configuration

Create `config.json`:

```json
{
  "backendEndpoint": "",
  "scriptRoot": "scripts",
  "maxPlayers": 10,
  "demoMode": false
}
```

Load order: config.json → environment → defaults

## Roadmap (v1)

- [x] Core event/action pipeline
- [x] Hardcoded runtime for demos
- [x] Scene management
- [x] Player registry
- [x] JSON configuration
- [ ] Lua script runtime
- [ ] Paper/Spigot integration
- [ ] Network protocol bridge

## License

MIT

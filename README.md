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

## Getting Started

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run

```bash
./build/mcx --demo        # Run fake event demo
./build/mcx --version     # Show version
```

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

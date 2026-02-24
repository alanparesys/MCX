# Changelog

All notable changes to MCX will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Windows x64 build support
- Lua runtime built-in (no optional dependency)
- Paper/Spigot plugin (Java)
- ARM64 support for Raspberry Pi

## [0.1.0] - 2024-02-24

### Added
- Initial prototype release
- 30+ core subsystems:
  - EventBus, Scheduler, ThreadScheduler, AsyncWorker
  - PlayerRegistry, SceneManager, PermissionRegistry
  - EconomyManager, TeleportManager, StatsManager
  - BanManager, ChatFilter, KitManager, WorldManager
  - BackupManager, FileLogger, HttpClient, WebSocketServer
  - RCON client/server, Protocol handler
  - ModuleLoader for DLL extensions
  - EntityManager (ECS), ConfigWatcher
- Interactive setup wizard (`--setup`)
- Demo mode (`--demo`)
- JSON configuration system
- CMake build system
- Sample DLL module
- Cross-platform headers (Linux/Windows)

### Security
- VPS hardening guide included
- Firewall (iptables) configuration
- SSH key-only authentication
- Fail2ban protection
- Auto-security updates

### Documentation
- Complete Mintlify documentation site
- API reference for all core classes
- Lua scripting guide
- Deployment guides (VPS, Docker, Nginx)

[Unreleased]: https://github.com/alanparesys/MCX/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/alanparesys/MCX/releases/tag/v0.1.0

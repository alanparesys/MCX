# MCX Releases

## Release Categories

### Stable Releases
Production-ready versions with full feature set and documentation.

| Version | Date | Status | Download |
|---------|------|--------|----------|
| [v0.1.0](https://github.com/alanparesys/MCX/releases/tag/v0.1.0) | 2024-02-24 | Current | [Linux x64](https://github.com/alanparesys/MCX/releases/download/v0.1.0/mcx-v0.1.0-linux-x64.tar.gz) |

### Beta Releases
Pre-release versions for testing new features.

| Version | Date | Status | Features |
|---------|------|--------|----------|
| v0.2.0-beta | TBD | Planned | Lua runtime, Windows support |

### Development Builds
Latest commits from main branch (unstable).

| Build | Date | Artifact |
|-------|------|----------|
| main-latest | Auto | [Download](https://github.com/alanparesys/MCX/actions) |

## Installation by Version

### Latest Stable (Recommended)

```bash
# Linux x64
curl -L -o mcx.tar.gz https://github.com/alanparesys/MCX/releases/latest/download/mcx-v0.1.0-linux-x64.tar.gz
tar xzvf mcx.tar.gz
./mcx --version
```

### Specific Version

```bash
# Replace X.X.X with desired version
curl -L -o mcx.tar.gz https://github.com/alanparesys/MCX/releases/download/vX.X.X/mcx-vX.X.X-linux-x64.tar.gz
```

### From Source (Any Version)

```bash
git clone https://github.com/alanparesys/MCX.git
cd MCX
git checkout v0.1.0  # or any tag
mkdir build && cd build
cmake .. && make -j$(nproc)
```

## Version Support Matrix

| Version | Linux | Windows | macOS | Lua | Status |
|---------|-------|---------|-------|-----|--------|
| v0.1.0 | ✅ x64 | ✅ x64 | ❌ | Optional | Stable |
| v0.2.0 | ✅ x64 | ✅ x64 | ❌ | Built-in | In Progress |
| v0.3.0 | ✅ x64/ARM | ✅ x64 | ❌ | Built-in | Planned |

## Release Cycle

- **Patch (0.0.X)**: Bug fixes, security updates
- **Minor (0.X.0)**: New features, enhancements
- **Major (X.0.0)**: Breaking changes, architectural updates

## Checksums

Verify downloads with SHA256:

```bash
sha256sum mcx-v0.1.0-linux-x64.tar.gz
# Expected: (see release page)
```

## Docker Images

```bash
# Latest stable
docker pull ghcr.io/alanparesys/mcx:latest

# Specific version
docker pull ghcr.io/alanparesys/mcx:v0.1.0
```

---

For detailed changelog, see [CHANGELOG.md](CHANGELOG.md)

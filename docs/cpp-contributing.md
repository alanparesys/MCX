# Contributing to MCX (C++ core)

This document is aimed at developers who want to work on the C++ core of MCX.

## Getting the code

```bash
git clone git@github.com:alanparesys/MCX.git
cd MCX
```

## Building the C++ core

Requirements:

- CMake 3.16+
- A C++20-capable compiler (for example `g++` 11+ or `clang++` 13+)

Basic build:

```bash
cmake -S . -B build
cmake --build build
```

This will build:

- `build/mcx` – prototype binary running the core and a small demo
- `build/mcx_tests` – minimal test binary for the core types

See `docs/build.md` and `docs/tests.md` for more details.

## Running the demo and tests

```bash
./build/mcx
./build/mcx_tests
```

The `mcx` binary currently runs a dummy `ScriptRuntime` and a fake event sequence. It is intended as a smoke test and a
starting point while the real server integration is being built.

## Branches and pull requests

The project uses feature branches for changes, even small ones.

Typical workflow:

```bash
git checkout main
git pull

git checkout -b feature/short-description
# edit files, build, run tests
git add ...
git commit -m "feat: ..."  # or chore:, docs:, etc.
git push -u origin feature/short-description
```

Then open a pull request from `feature/short-description` to `main` on GitHub.

### Commit messages

Use short, descriptive messages with a prefix when possible, for example:

- `feat: add ScriptRuntime interface`
- `chore: tidy includes and clarify core headers`
- `docs: update Lua API design`

## Where to start

Some good entry points into the C++ code:

- `include/mcx/events.hpp` and `include/mcx/actions.hpp` – core types for events and actions.
- `include/mcx/server.hpp` and `src/server.cpp` – main server core.
- `include/mcx/script_runtime.hpp` and `src/script_runtime_dummy.cpp` – placeholder for the future scripting runtime.

Design documents in `docs/` provide context:

- `architecture-v1.md` – how MCX fits between Minecraft and external backends.
- `cpp-core.md` – overview of the current C++ core.
- `implementation-notes.md` – notes about the long-term C++ + Lua direction.

## Style reminders

See the project-wide rules, but in short:

- Filenames: `snake_case`
- Variables: `camelCase`
- Functions: `PascalCase` (except `main`)
- Constants and enum values: `ALL_CAPS`
- Structs / classes / enums: `PascalCase`

Keep functions small, names explicit, and avoid unnecessary complexity.

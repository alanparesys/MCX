# Building MCX (C++)

MCX uses CMake for its C++ build.

## Requirements

- CMake 3.16+
- A C++20-capable compiler (for example `g++` 11+ or `clang++` 13+)

## Basic build

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

This will produce at least two binaries:

- `build/mcx` – the main MCX prototype executable
- `build/mcx_tests` – a small test binary for the core types

## Running the main binary

```bash
./build/mcx
```

This will:

- start the MCX C++ core,
- log basic configuration information,
- run a small fake event sequence (until a real Minecraft integration is wired in).

## Running tests

See `docs/tests.md` for details, but in short:

```bash
./build/mcx_tests
```

As MCX grows, the build and test setup may evolve (for example, adding a proper test framework). This document will be
updated accordingly.

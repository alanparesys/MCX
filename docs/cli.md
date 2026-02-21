# MCX CLI

The `mcx` binary is a small command-line entrypoint for the MCX C++ core. At this stage it is intentionally minimal
but is structured so more commands can be added over time.

## Usage

From the repository root (after building):

```bash
./build/mcx --demo
./build/mcx --version
```

### `--demo`

Runs the current demo mode:

- starts the MCX core,
- logs basic configuration,
- runs a small fake event sequence so the core behavior can be observed without a real Minecraft server.

### `--version`

Prints a simple version string. For now this is hard-coded, but it can later be wired to build metadata
(commit hashes, tags, etc.).

If no arguments are provided, `mcx` will print a short usage message.

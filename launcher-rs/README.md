# MCX Rust Launcher (Sprint 3)

This folder contains the Rust launcher prototype for Sprint 3.

## Current scope

- Query latest release from GitHub
- Compare semantic versions safely (`v1.1.0` vs `v1.0.9`)
- Pick a platform-appropriate asset automatically
- Download latest asset to a target file
- Install latest asset into a local runtime directory
- Persist launcher state (`launcher-state.json`)
- Launch installed MCX executable

## Commands

```bash
cargo run -- check --current v1.1.0
cargo run -- download --output ./mcx-latest.bin
cargo run -- install --dir ./.mcx-runtime
cargo run -- launch --dir ./.mcx-runtime -- --server
```

## Notes

- This is still a standalone prototype and does not replace the Qt launcher yet.
- Sprint 4 target: UI shell (Tauri/egui) and rollback-safe updater flow.

# MCX Rust Launcher (Sprint 2)

This folder contains the Sprint 2 prototype for a Rust-based MCX launcher.

## Current scope

- Query latest release from GitHub
- Compare semantic versions safely (`v1.1.0` vs `v1.0.9`)
- Pick a platform-appropriate asset automatically
- Download latest asset to a target file

## Commands

```bash
cargo run -- check --current v1.1.0
cargo run -- download --output ./mcx-latest.bin
```

## Notes

- This is a standalone prototype and does not replace the Qt launcher yet.
- Next step (Sprint 3): UI shell + update/install flow + launch integration.

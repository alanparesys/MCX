# MCX Implementation Notes

This document captures early thoughts about how MCX should be implemented internally.

## Long-term implementation language

The long-term goal for MCX is:

- **Core implementation in C++**, for performance and fine-grained control over resources.
- **Scripting surface in Lua**, exposed to creators as the main way to define behavior (modes, rules, scenes).

This means that, in the future, the MCX runtime is expected to be:

- a C++ application/library that:
  - speaks to Minecraft servers/clients (directly or via existing protocols),
  - hosts a Lua runtime (for example LuaJIT or standard Lua),
  - exposes a clean C++ ↔ Lua boundary for events and actions.

## Why there is TypeScript in the repository

MCX is still in the design/prototype phase. The TypeScript code currently present in the repository serves to:

- express and refine **core models** (events, actions, configuration),
- sketch how a server-side component might look,
- experiment with structure before committing to a full C++ implementation.

This code is intentionally small and should be considered **disposable** once the C++ core is ready to take over. The
important part is the *shape* of the models and the behavior, not the specific language used in early prototypes.

## C++ + Lua direction (high level)

When MCX moves to C++, a likely architecture will be:

- A C++ core that:
  - manages connections to the Minecraft server or protocol,
  - tracks players, scenes, and internal state,
  - loads and reloads Lua scripts on demand.
- An embedded Lua runtime that:
  - provides the `MCX` API to scripts (`MCX.on`, `MCX.sendMessage`, etc.),
  - receives events from C++ and returns actions for C++ to apply,
  - is kept sandboxed and focused on gameplay logic.

The existing docs (`lua-api-design.md`, `architecture-v1.md`, `scenes-concept.md`) are written with this direction in
mind, even if the very first experiments use other technologies.

As the project evolves, this document should be updated to reflect concrete implementation choices on the C++ side
(tooling, build system, Lua integration strategy, etc.).

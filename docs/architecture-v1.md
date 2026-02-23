# MCX Architecture – V1 Prototype

> This document describes a first-pass architecture for MCX V1. The goal is to capture how MCX fits between a
> Minecraft server and an optional external backend for the initial prototype.

## High-level picture

For V1, MCX is **not** a replacement for the Minecraft server. Instead, it runs alongside an existing server and
observes/controls certain aspects of it.

```text
Minecraft client(s)  <---->  Minecraft server (Paper/Spigot)  <---->  MCX component  <---->  External backend (optional)
```

- **Minecraft client(s)**: standard vanilla clients used by players.
- **Minecraft server**: a test server instance (for example Paper) that hosts the world.
- **MCX component**: the first MCX piece we will implement, running on the same machine as the server.
- **External backend (optional)**: a service that can receive events and respond with suggestions or actions (for example, for moderation, logging, or automation).

## MCX component responsibilities (V1)

The MCX component for V1 should:

- Listen to a **small set of events** from the server:
  - player join / quit
  - chat messages
  - commands
- Forward selected events to an external backend in a simple, well-defined format.
- Apply **simple actions** in response to the backend outputs, for example:
  - sending messages to players
  - broadcasting messages to the server
  - logging information for the admin

MCX V1 does *not* need to:

- modify chunks or world data directly,
- handle complex NPCs or pathfinding,
- manage assets (models, resource packs).

Those topics are reserved for later versions.

## Communication between MCX and the server

Two main implementation options exist for V1:

1. **Plugin approach** (Paper/Spigot plugin)
2. **Companion service approach** (separate process talking via RCON/websocket/other)

### Plugin approach

- MCX is packaged as a plugin running inside the Minecraft server JVM.
- Advantages:
  - direct access to server events and API (join, chat, commands, teleport, etc.),
  - simpler to prototype event handling.
- Disadvantages:
  - tighter coupling to a specific server implementation (Paper/Spigot),
  - harder to migrate later to a proxy/standalone architecture.

### Companion service approach

- MCX runs as a separate process (for example Node/TypeScript or another JVM process) and communicates with the
  Minecraft server using an integration channel:
  - RCON,
  - a small plugin that forwards events over HTTP/websocket,
  - or another lightweight bridge.
- Advantages:
  - better separation between MCX and the underlying server,
  - easier to evolve MCX towards a proxy/standalone architecture later.
- Disadvantages:
  - slightly more moving pieces for the prototype,
  - requires a small bridge to extract events from the server.

For V1 we can choose whichever path is easier to iterate on given existing tooling and time constraints. The important
part is that the **MCX internal model of events and actions** matches what we defined in `docs/lua-api-design.md`.

## Communication between MCX and external backends

For the first prototype, MCX does not need a complex protocol to talk to an external backend. A simple request/response pattern is
sufficient:

- MCX sends an event payload to the backend (for example via HTTP or websocket), including:
  - event type (`player_join`, `chat`, etc.),
  - relevant fields (player, message, scene, etc.),
  - optional context.
- External backend returns a response that MCX can translate into actions:
  - send a message,
  - broadcast something,
  - log a suggestion for the admin.

Example (conceptual, not final):

```json
{
  "type": "chat",
  "player": {
    "id": "uuid-1234",
    "name": "NorthTheval"
  },
  "message": "!scene test"
}
```

The backend could return:

```json
{
  "actions": [
    { "kind": "broadcast", "text": "Switching to test scene (requested by NorthTheval)." },
    { "kind": "loadScene", "name": "test" }
  ]
}
```

MCX V1 only needs to support a **very small subset** of actions. More complex behavior can be added once the basic
loop is stable.

## Where Lua fits (later)

In V1, MCX may not execute Lua scripts yet. Instead, it can:

- observe events,
- optionally call an external backend,
- perform hard-coded actions.

However, the architecture should already anticipate that:

- events will eventually be delivered to a Lua runtime,
- actions will eventually be requested by Lua scripts via the MCX API.

This is why the event payloads and actions for V1 should follow the shapes and naming we defined in
`docs/lua-api-design.md`.

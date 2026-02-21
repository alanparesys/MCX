# MCX Lua API (early design)

> This document describes an early design for the MCX Lua scripting API. Nothing here is final; the goal is to capture
> the shape of the runtime we want to offer to MCX creators.

## Goals

- Simple, readable API for scripters who know basic Lua.
- Event-driven: scripts react to what happens on the server (players, chat, world events).
- Explicit actions: scripts call MCX functions to affect the game (messages, scenes, teleport, etc.).
- Decoupled from Minecraft internals: scripts work with MCX concepts (players, scenes, roles) rather than raw packets.

## Core concepts

### Players

Scripts see players as objects with at least:

- `player.id` – stable identifier on the server
- `player.name` – display name / username
- `player.position` – current position (optional shorthand)

### Scenes

A scene is a higher-level description of a situation or setup in the world. For the first prototypes this can be as
simple as a named configuration (map, rules, roles).

- `MCX.loadScene(name)` – switch to a named scene
- `MCX.currentScene()` – returns the identifier of the active scene

### Events

MCX forwards server events into Lua as high-level callbacks.

Proposed events (V1+V2):

- `"player_join"` – a player has joined the server
- `"player_quit"` – a player has left the server
- `"chat"` – a player sent a chat message
- `"command"` – a player or console used a command
- `"scene_loaded"` – a scene has been loaded/activated

Example registration pattern:

```lua
MCX.on("player_join", function(player)
  MCX.sendMessage(player, "Welcome to MCX, " .. player.name .. "!")
end)

MCX.on("chat", function(player, message)
  if message == "!scene test" then
    MCX.loadScene("test")
    MCX.broadcast("Scene 'test' loaded.")
  end
end)
```

## Proposed Lua API surface

All functions are namespaced under a global `MCX` table to avoid polluting the Lua global namespace.

### Event registration

```lua
MCX.on(eventName, handler)
```

- `eventName` – string, one of the supported events (`"player_join"`, `"chat"`, etc.)
- `handler` – function invoked when the event occurs

### Messaging

```lua
MCX.sendMessage(player, text)
MCX.broadcast(text)
MCX.sendTitle(player, title, subtitle)
```

- `player` – player object provided by MCX
- `text` / `title` / `subtitle` – strings (may support basic formatting later)

### Players and movement

```lua
MCX.teleport(player, position)
MCX.setSpawn(player, position)
MCX.getOnlinePlayers()
```

- `position` – table `{ x = number, y = number, z = number }`
- `MCX.getOnlinePlayers()` returns a list/array of player objects.

### Scenes

```lua
MCX.loadScene(name)
MCX.currentScene()
MCX.listScenes()
```

- `name` – identifier of a scene known to the server
- `MCX.currentScene()` returns the currently active scene name (or `nil`)
- `MCX.listScenes()` returns a list of available scene identifiers

### Roles / tags (optional, later)

```lua
MCX.setRole(player, roleName)
MCX.getRole(player)
MCX.addTag(player, tag)
MCX.hasTag(player, tag)
MCX.removeTag(player, tag)
```

This is a higher-level concept for roleplay / game modes: roles and tags are purely MCX metadata unless mapped to
server permissions.

## Example script: simple lobby

```lua
-- scripts/lobby.lua

-- When a player joins, send a welcome message and teleport them to the lobby spawn.
MCX.on("player_join", function(player)
  MCX.sendMessage(player, "Welcome to the MCX lobby, " .. player.name .. "!")
  MCX.teleport(player, { x = 0, y = 64, z = 0 })
end)

-- Simple command-like chat trigger to load a test scene.
MCX.on("chat", function(player, message)
  if message == "!test" then
    MCX.loadScene("test_lobby")
    MCX.broadcast("[MCX] Switched to test_lobby scene.")
  end
end)
```

## Notes

- This design is intentionally small and focused. The goal is to have a Lua surface that is easy to implement in a
  first prototype while leaving room for more advanced features later (NPCs, world editing, assets, etc.).
- Nothing here enforces how scenes are stored or loaded; that is an internal concern of the MCX server/runtime. From the
  Lua perspective, scenes are just named handles that can be loaded and queried.

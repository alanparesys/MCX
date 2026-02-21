# MCX Scenes – Concept

> Scenes are a core concept for MCX. This document describes what a scene is, why it exists, and how it might evolve
> over time.

## What is a scene?

In MCX, a **scene** is a named configuration that describes a particular setup or experience on the server. A scene can
include:

- which world or map is active,
- where players spawn,
- which rules or game mode are enabled,
- which roles or teams exist,
- optional scripted behavior (events, transitions).

A scene is not necessarily a single physical map: it can be thought of as a **state of the server** (or a subset of it)
that MCX can load, modify, and switch between.

## Why scenes instead of only worlds?

Vanilla Minecraft already has worlds (overworld, nether, etc.) and server admins are used to managing multiple worlds.
MCX scenes sit at a higher level:

- A scene can choose which world(s) are relevant.
- A scene can bundle rules, roles, and behaviors with the world.
- Scripts can switch scenes as part of gameplay (for example, moving from a lobby to an arena).

This allows MCX to express concepts like:

- "Lobby scene" (minimal rules, no PvP, everyone spawns in a hub),
- "City RP scene" (specific regions, jobs, economy rules),
- "Dungeon run scene" (instanced area, specific mobs, victory/defeat conditions).

## Scenes in V1

For MCX V1, scenes can be kept deliberately simple:

- A scene is identified by a **name** (string).
- MCX can:
  - load a scene by name,
  - report which scene is currently active,
  - list available scenes.

The actual implementation details (how worlds, rules, or data are managed) can be minimal or even stubbed out. What
matters is that:

- there is a clear API for scripts and for MCX itself:
  - `MCX.loadScene(name)`
  - `MCX.currentScene()`
  - `MCX.listScenes()`
- scenes are treated as first-class concepts in the design.

## How scenes could evolve

Over time, scenes can become richer and more structured. For example, a scene definition might look like this
(eventually):

```yaml
name: city_rp
world: city_world
spawn: { x: 0, y: 64, z: 0 }
maxPlayers: 40
rules:
  pvp: false
  keepInventory: true
  economy: enabled
roles:
  - name: citizen
    default: true
  - name: police
    permissions:
      - arrest_players
      - access_station
scripts:
  onEnter: city/on_enter.lua
  onExit: city/on_exit.lua
```

MCX does not need to support this full richness in early versions, but the concept of **named scenes that bundle
configuration and behavior** is important from the start.

## Scenes and AI

Scenes are also a useful hook for AI integration:

- An AI assistant can help design or adjust scenes:
  - propose rules and roles for a given theme,
  - suggest spawn locations or basic flows.
- MCX can send scene context to OpenClaw when asking for help:

```json
{
  "type": "scene_context",
  "scene": "city_rp",
  "rules": { "pvp": false, "economy": "enabled" },
  "roles": ["citizen", "police"]
}
```

This allows OpenClaw to provide answers and actions that are aware of the current experience, not just raw chat.

## Summary

- Scenes are MCX's way of describing **experiences** built on top of Minecraft.
- Even in V1, treating scenes as named entities with a minimal API (`loadScene`, `currentScene`, `listScenes`) gives a
  clear direction for future work.
- As MCX grows, scenes can gradually accumulate more structure (worlds, rules, roles, scripts) without breaking the
  basic mental model.

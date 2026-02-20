# MCX

MCX is an open-source platform that aims to extend Minecraft without modifying the original game client. It focuses on building a modern, scriptable layer around Minecraft servers, with deep integration of AI through OpenClaw.

## Vision

- Allow creators to build rich, custom gameplay experiences on top of Minecraft.
- Keep the vanilla client experience: no custom launcher, no client-side code changes required for the first versions.
- Use AI to help with moderation, content generation, scenario design, and server automation.

## Core ideas

- **Server-side first**: MCX runs alongside or inside existing Minecraft server stacks (for example via plugins or a network proxy).
- **Scenes and experiences**: instead of editing raw world data manually, MCX exposes higher-level concepts like scenes, characters, and rules.
- **AI-assisted workflows**: MCX connects to OpenClaw so that creators and admins can use an AI assistant to:
  - generate or tweak scenes, events, and quests,
  - assist with moderation and logging,
  - help design and test new experiences.

## First prototype (v1 direction)

- A server-side component (plugin or companion service) that can:
  - receive events from a Minecraft server (joins, chat, commands, world events),
  - expose a simple API for scenes (load, save, switch) and characters,
  - talk to an MCX backend that is powered by OpenClaw.
- Basic integrations:
  - command interface for admins (in-game or via Discord) to control MCX,
  - a minimal scene system that can load predefined configurations (maps, rules, roles).

## Long-term direction

- A more advanced server/proxy layer that understands the Minecraft protocol and can:
  - orchestrate complex experiences across multiple worlds/servers,
  - inject AI-driven behaviors for NPCs and events,
  - provide a clean extension API so other developers can build on top of MCX.
- A strong focus on documentation and openness so that contributors can understand, extend, and maintain MCX over time.

MCX is designed to be a project where experimentation, AI, and Minecraft meet. It does not aim to replace the original game, but to give creators a modern toolbox to build new experiences on top of it.

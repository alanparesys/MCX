# MCX – Positioning and Scope

MCX is an open-source platform that sits **on top of Minecraft servers**. It is not a replacement for the game, but a
layer that makes it easier to build and run rich, AI-powered experiences.

This document clarifies what MCX is, what it is not, and how it positions itself among existing tools.

## What MCX is

- **A server-side experience layer** for Minecraft.
  - MCX focuses on the server side: events, rules, scenes, and automation.
  - The initial prototypes run alongside an existing server (for example Paper), with the long-term goal of a more
    autonomous MCX layer.

- **A scripting and orchestration platform.**
  - MCX is designed to expose a clear scripting API (starting with Lua) so creators can write modes, rules, and
    behaviors as scripts.
  - The goal is to let people describe experiences in code and configuration, not by manually editing worlds forever.

- **AI-aware by design.**
  - MCX integrates with OpenClaw from the start.
  - AI is not an afterthought or a plugin: it is part of how MCX helps with moderation, content generation, and
    scenario design.

- **Open source and community-oriented.**
  - MCX is intended to be fully open source and free to use.
  - The long-term vision is a community of server owners, scripters, and contributors building on the same toolbox.

## What MCX is not

- **Not a fork of the Minecraft client.**
  - MCX does not modify the original game client.
  - Players use a normal Minecraft client; MCX lives on the server side (and in tooling around it).

- **Not just "another plugin".**
  - MCX uses existing server platforms (like Paper/Spigot) as a starting point for prototyping, but its scope is
    broader.
  - The ambition is to provide a consistent experience model (events, scenes, scripts) that can outlive a single server
    implementation.

- **Not a replacement for existing modding ecosystems.**
  - MCX does not try to make all other plugins or modpacks obsolete.
  - Instead, it aims to complement existing setups by providing a higher-level layer focused on experiences and AI.

## Target users

- **Server owners** who want more control and more expressive tools without maintaining a full custom codebase.
- **Scripters and creators** who prefer writing logic in Lua or a similar scripting language rather than Java.
- **Tool builders** who want to integrate Minecraft with other systems (Discord, web dashboards, analytics, etc.) and
  need a clear API.

## Why AI integration matters

AI is not just a buzzword in MCX; it is a practical tool for:

- **Moderation and logging** – helping admins understand what is happening on the server and react faster.
- **Scenario and quest design** – generating ideas, descriptions, and small building blocks for scenes.
- **Automation** – reacting to patterns in gameplay, server load, or player behavior.

OpenClaw acts as a multi-agent backend that MCX can send events to and receive structured suggestions or actions from.

## Long-term vision in one paragraph

MCX wants to be the place where **Minecraft, scripting, and AI meet**. It is a toolbox for building experiences rather
than a single mode or minigame. Over time, the goal is for MCX to provide:

- a stable event and action model,
- a well-designed scripting API (starting with Lua),
- integrations with AI backends,
- and a strong documentation story,

so that other developers and creators can build serious, long-lived projects on top of it.

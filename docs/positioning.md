# MCX – Positioning and Scope

MCX is an open-source platform that sits **on top of Minecraft servers**. It is not a replacement for the game, but a
layer that makes it easier to build and run rich, scripted experiences.

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

- **Scriptable and automation-friendly by design.**
  - MCX is designed to integrate with external services and tooling when needed.
  - Automation, moderation, and scenario design are driven by scripts and clear server-side APIs.

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

## Why integrations matter

External services and tools are not just a buzzword in MCX; they are practical options for:

- **Moderation and logging** – helping admins understand what is happening on the server and react faster.
- **Scenario and quest design** – generating ideas, descriptions, and building blocks for scenes.
- **Automation** – reacting to patterns in gameplay, server load, or player behavior.

MCX is built so that it can send events to other systems and receive structured suggestions or actions in return.

## Long-term vision in one paragraph

MCX wants to be the place where **Minecraft meets scripting and external services**. It is a toolbox for building experiences rather
than a single mode or minigame. Over time, the goal is for MCX to provide:

- a stable event and action model,
- a well-designed scripting API (starting with Lua),
- integrations with external services,
- and a strong documentation story,

so that other developers and creators can build serious, long-lived projects on top of it.

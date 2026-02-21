# MCX Runtime Design (C++ + Lua)

This document expands on `implementation-notes.md` and focuses on how the MCX runtime is expected to work internally
once Lua scripting is integrated on top of the C++ core.

## Goals

- Keep the **C++ core** responsible for state, performance-critical paths, and integration with Minecraft.
- Use **Lua** as the main scripting language for gameplay logic, scenes, and modes.
- Provide a **clear boundary** between C++ and Lua so that scripts are easy to write and the core remains robust.

## High-level architecture

At a high level:

```text
Minecraft server  <---->  MCX C++ core  <---->  Lua runtime  <---->  MCX scripts
```

- The Minecraft server sends events (join/quit/chat/commands, etc.) into MCX.
- The C++ core translates these into `Event` structures and passes them to the scripting runtime.
- Lua scripts receive events through the `MCX` API and return actions.
- The C++ core applies the resulting `ActionList` to the server.

## ScriptRuntime interface

The C++ side sees scripting through the `ScriptRuntime` interface (`include/mcx/script_runtime.hpp`):

```cpp
class ScriptRuntime {
public:
    virtual ~ScriptRuntime() = default;

    virtual void LoadScripts(const std::string& rootPath) = 0;
    [[nodiscard]] virtual ActionList HandleEvent(const Event& event) = 0;
};
```

A concrete Lua-based implementation will:

- embed a Lua interpreter (LuaJIT or standard Lua),
- load and evaluate `.lua` files from a script root directory,
- register handlers for events (for example via `MCX.on` in Lua),
- map incoming `Event` values to Lua tables,
- collect and map actions requested by Lua back into `Action` structures.

For now, `DummyScriptRuntime` is only a placeholder that logs script loading and returns an empty `ActionList`.

## Event flow from C++ to Lua

1. The C++ core receives an `Event` from some integration (for example via `MinecraftBridge`).
2. `Server::HandleEvent` is called with that `Event`.
3. The server first logs the event (for demo) and then calls `scriptRuntime_->HandleEvent(event)`.
4. The Lua-based runtime will:
   - convert `Event` into a Lua table,
   - dispatch it to registered Lua handlers,
   - collect the actions they request.

Example Lua handler (from `docs/lua-api-design.md`):

```lua
MCX.on("chat", function(player, message)
  if message == "!test" then
    MCX.loadScene("test_lobby")
    MCX.broadcast("[MCX] Switched to test_lobby scene.")
  end
end)
```

On the C++ side this might correspond to:

- an `Event` of type `EVENT_TYPE::CHAT` with `ChatEvent` payload,
- a Lua call where `player` is a Lua table and `message` is a string,
- Lua code calling `MCX.loadScene` and `MCX.broadcast`.

## Actions from Lua back to C++

In Lua, the `MCX` API will expose functions that **do not act directly on the server**, but instead
record actions for the runtime to return to C++.

Conceptually:

```lua
-- inside Lua runtime implementation
function MCX.broadcast(text)
  runtime_add_action({ kind = "broadcast", text = text })
end
```

On the C++ side, the Lua runtime will:

- maintain an internal list of pending actions,
- after running handlers for an event, convert that list into `ActionList`:
  - `SEND_MESSAGE` / `BROADCAST` / `TELEPORT` / `LOAD_SCENE`, etc.
- return the `ActionList` to `Server::HandleEvent`.

The server can then:

- apply these actions to the Minecraft server (via whatever integration exists),
- log them, or modify them if needed.

## Error handling and isolation

Some initial guidelines for the Lua runtime implementation:

- Lua errors during handler execution should **not crash** the C++ core.
- A failing script should result in a logged error and an empty `ActionList` for that event.
- Scripts should be kept focused on gameplay logic; heavy operations or external I/O should remain in C++ or other
  services.

## Next steps for implementation

- Choose a specific Lua implementation (standard Lua vs LuaJIT) and integration strategy.
- Implement a first concrete `ScriptRuntime` that:
  - embeds Lua,
  - registers a minimal `MCX` API (for example `MCX.on` and `MCX.sendMessage`),
  - supports one or two events end-to-end as a prototype.
- Keep the interface and types aligned with `docs/lua-api-design.md` so that scripts stay consistent when the
  implementation evolves.

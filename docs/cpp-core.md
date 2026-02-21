# MCX C++ Core – Overview

This document summarizes the current C++ core of MCX. The goal is to keep the core small, explicit, and easy to
understand for contributors.

## Key types

### `Config`

Defined in `include/mcx/server.hpp`.

```cpp
struct Config {
    std::string backendEndpoint; // optional external backend; may be empty
};
```

`Config` holds configuration options for the MCX server core. For now there is only one field, `backendEndpoint`, which
can be used to point to an external service that receives events or suggestions. It may be left empty.

### `Player` and `Vec3`

Defined in `include/mcx/events.hpp`.

```cpp
struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct Player {
    std::string id;
    std::string name;
    std::optional<Vec3> position;
};
```

These are the basic building blocks for representing players and positions in MCX events.

### `EVENT_TYPE` and `Event`

Also in `include/mcx/events.hpp`.

```cpp
enum class EVENT_TYPE {
    PLAYER_JOIN,
    PLAYER_QUIT,
    CHAT,
    COMMAND,
    SCENE_LOADED,
};

struct Event {
    EVENT_TYPE type{EVENT_TYPE::PLAYER_JOIN};
    std::optional<PlayerJoinEvent> playerJoin;
    std::optional<PlayerQuitEvent> playerQuit;
    std::optional<ChatEvent> chat;
    std::optional<CommandEvent> command;
    std::optional<SceneLoadedEvent> sceneLoaded;
};
```

`EVENT_TYPE` enumerates the high-level events MCX cares about. `Event` is a small tagged container used in early
prototypes to pass events into the MCX core. Later versions may use a more advanced variant type or a different
representation.

### `ACTION_KIND` and `Action`

Defined in `include/mcx/actions.hpp`.

```cpp
enum class ACTION_KIND {
    SEND_MESSAGE,
    BROADCAST,
    SEND_TITLE,
    TELEPORT,
    SET_SPAWN,
    LOAD_SCENE,
};

using Action = std::variant<
    SendMessageAction,
    BroadcastAction,
    SendTitleAction,
    TeleportAction,
    SetSpawnAction,
    LoadSceneAction
>;

using ActionList = std::vector<Action>;
```

`ACTION_KIND` enumerates the kinds of actions MCX can request from the underlying server. `Action` is a `std::variant`
that can hold any of the supported action structs. `ActionList` is simply a list of actions.

## `Server` class

Defined in `include/mcx/server.hpp` and implemented in `src/server.cpp`.

```cpp
class Server {
public:
    explicit Server(Config config = {});

    void Start();

    [[nodiscard]] ActionList HandleEvent(const Event& event);

private:
    Config config_;
};
```

- `Start()` is responsible for initializing the MCX core and printing basic startup information.
- `HandleEvent()` receives an `Event` from the outside world (for example from a Minecraft server integration) and
  returns a list of actions MCX would like to perform.

In the current prototype, `HandleEvent()` only logs the event type and returns an empty list. As MCX evolves, this is
where routing to scripting (Lua) and external backends will be integrated.

## Helper functions (future work)

To keep code clean and avoid repetition, future work may introduce small helper functions such as:

- factories for building `Event` instances from raw data,
- utilities for inspecting or logging events and actions.

These helpers should live in small, focused headers/source files and follow the same naming/style rules as the rest of
MCX.

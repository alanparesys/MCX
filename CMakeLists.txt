cmake_minimum_required(VERSION 3.16)
project(MCX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mcx_core
    src/core/server.cpp
    src/core/script_runtime_dummy.cpp
    src/core/script_runtime_hardcoded.cpp
    src/core/event_utils.cpp
    src/core/action_utils.cpp
    src/core/action_applier.cpp
    src/core/log.cpp
    src/core/scene_manager.cpp
    src/core/player_registry.cpp
    src/integration/fake_event_source.cpp
    src/integration/minecraft_bridge_dummy.cpp
)

target_include_directories(mcx_core PUBLIC include)

add_executable(mcx
    src/main.cpp
)

target_link_libraries(mcx PRIVATE mcx_core)

add_executable(mcx_tests
    tests/test_core.cpp
)

target_link_libraries(mcx_tests PRIVATE mcx_core)

cmake_minimum_required(VERSION 3.16)
project(MCX LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Lua QUIET)
find_package(Threads REQUIRED)

set(MCX_CORE_SOURCES
    src/core/server.cpp
    src/core/module_loader.cpp
    src/core/thread_scheduler.cpp
    src/core/scene_manager.cpp
    src/core/player_registry.cpp
    src/core/scheduler.cpp
    src/core/command_parser.cpp
    src/core/signal_handler.cpp
    src/core/config_validator.cpp
    src/core/file_logger.cpp
    src/core/permissions.cpp
    src/core/action_applier.cpp
    src/core/log.cpp
    src/core/event_utils.cpp
    src/core/script_runtime_dummy.cpp
    src/core/script_runtime_hardcoded.cpp
    src/core/event_bus.cpp
    src/core/command_registry.cpp
    src/core/config_watcher.cpp
    src/core/packet_serializer.cpp
    src/core/entity_manager.cpp
    src/core/console.cpp
    src/integration/paper_bridge.cpp
    src/integration/network_bridge.cpp
    src/integration/fake_event_source.cpp
    src/integration/minecraft_bridge_dummy.cpp
)

if(LUA_FOUND)
    list(APPEND MCX_CORE_SOURCES src/core/lua_runtime.cpp)
    message(STATUS "Lua found - enabling Lua runtime")
else()
    message(STATUS "Lua not found - Lua runtime disabled")
endif()

add_library(mcx_core ${MCX_CORE_SOURCES})

target_include_directories(mcx_core
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(LUA_FOUND)
    target_include_directories(mcx_core PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(mcx_core PRIVATE ${LUA_LIBRARIES})
    target_compile_definitions(mcx_core PRIVATE MCX_HAS_LUA)
endif()

target_compile_definitions(mcx_core PRIVATE MCX_CORE_EXPORTS)

target_link_libraries(mcx_core
    PUBLIC
        Threads::Threads
)

add_executable(mcx_server src/main.cpp)
target_link_libraries(mcx_server PRIVATE mcx_core)
set_target_properties(mcx_server PROPERTIES OUTPUT_NAME "mcx")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

add_executable(mcx_tests tests/test_core.cpp)
target_link_libraries(mcx_tests PRIVATE mcx_core)

# Sample module
add_library(sample_module SHARED examples/modules/sample_module.cpp)
target_link_libraries(sample_module PRIVATE mcx_core)
set_target_properties(sample_module PROPERTIES
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)

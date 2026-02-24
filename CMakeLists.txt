cmake_minimum_required(VERSION 3.16)
project(MCX LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-DNOMINMAX -DWIN32_LEAN_AND_MEAN)
    set(CMAKE_CXX_VISIBILITY_PRESET default)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN NO)
else()
    # Linux/Unix settings
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

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
    src/core/setup_wizard.cpp
    src/core/ban_manager.cpp
    src/core/chat_filter.cpp
    src/core/backup_manager.cpp
    src/core/http_client.cpp
    src/core/websocket_server.cpp
    src/core/world_manager.cpp
    src/core/economy_manager.cpp
    src/core/teleport_manager.cpp
    src/core/stats_manager.cpp
    src/core/async_worker.cpp
    src/core/kit_manager.cpp
    src/core/rcon.cpp
    src/core/protocol.cpp
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

if(WIN32 AND BUILD_SHARED_LIBS)
    add_library(mcx_core SHARED ${MCX_CORE_SOURCES})
else()
    add_library(mcx_core STATIC ${MCX_CORE_SOURCES})
    if(WIN32)
        target_compile_definitions(mcx_core PUBLIC MCX_STATIC)
    endif()
endif()

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

# Windows-specific libraries
if(WIN32)
    target_link_libraries(mcx_core PUBLIC ws2_32 wsock32)
endif()

add_executable(mcx_server src/main.cpp)
target_link_libraries(mcx_server PRIVATE mcx_core)
set_target_properties(mcx_server PROPERTIES OUTPUT_NAME "mcx")

if(WIN32)
    set_target_properties(mcx_server PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

add_executable(mcx_tests tests/test_core.cpp)
target_link_libraries(mcx_tests PRIVATE mcx_core)

# Sample module
add_library(sample_module SHARED examples/modules/sample_module.cpp)
target_link_libraries(sample_module PRIVATE mcx_core)

if(WIN32)
    set_target_properties(sample_module PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
    )
else()
    set_target_properties(sample_module PROPERTIES
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
    )
endif()

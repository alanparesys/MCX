cmake_minimum_required(VERSION 3.16)
project(MCX LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

if(BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

find_package(Lua REQUIRED)
find_package(Threads REQUIRED)

add_library(mcx_core
    src/core/server.cpp
    src/core/lua_runtime.cpp
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
    src/core/script_runtime_hardcoded.cpp
    src/integration/paper_bridge.cpp
    src/integration/network_bridge.cpp
    src/integration/fake_event_source.cpp
    src/integration/minecraft_bridge_dummy.cpp
)

target_include_directories(mcx_core
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${LUA_INCLUDE_DIR}
)

target_compile_definitions(mcx_core PRIVATE MCX_CORE_EXPORTS)

target_link_libraries(mcx_core
    PUBLIC
        Threads::Threads
    PRIVATE
        ${LUA_LIBRARIES}
)

add_executable(mcx_server src/main.cpp)
target_link_libraries(mcx_server PRIVATE mcx_core)
set_target_properties(mcx_server PROPERTIES OUTPUT_NAME "mcx")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

add_executable(mcx_tests tests/test_core.cpp)
target_link_libraries(mcx_tests PRIVATE mcx_core)

# MCX Launcher GUI (Qt5)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(mcx_launcher
    mcx_launcher.cpp
    mcx_launcher.hpp
)

target_link_libraries(mcx_launcher PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    mcx_core
)

set_target_properties(mcx_launcher PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME "mcx-gui"
)

# Windows-specific Qt deployment
if(WIN32)
    # Copy Qt DLLs on Windows
    add_custom_command(TARGET mcx_launcher POST_BUILD
        COMMAND ${Qt5Core_DIR}/../../../bin/windeployqt.exe $<TARGET_FILE:mcx_launcher>
        COMMENT "Deploying Qt dependencies..."
    )
endif()

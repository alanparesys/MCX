# MCX ImGui Launcher

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.0 REQUIRED)

# Download and setup ImGui
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.1  # Stable version
)
FetchContent_MakeAvailable(imgui)

# ImGui source files
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# ImGui headers
set(IMGUI_HEADERS
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imstb_rectpack.h
    ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
)

# Create ImGui library
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

# MCX ImGui Launcher
add_executable(mcx-gui
    mcx_imgui_launcher.cpp
    mcx_imgui_launcher.hpp
)

target_link_libraries(mcx-gui PRIVATE
    imgui
    mcx_core
    glfw
    OpenGL::GL
    Threads::Threads
)

# Add include for update_checker
# target_include_directories(mcx-gui PRIVATE ../core)

set_target_properties(mcx-gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME "mcx-gui"
)

# Copy to release directory
add_custom_command(TARGET mcx-gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "ImGui launcher built successfully"
)

message(STATUS "ImGui launcher will be built: mcx-gui")

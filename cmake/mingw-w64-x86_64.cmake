# Toolchain for cross-compiling Windows binaries on Linux
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake ..

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

# Find MinGW-w64 compiler
find_program(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
find_program(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
find_program(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
find_program(CMAKE_AR x86_64-w64-mingw32-ar)
find_program(CMAKE_RANLIB x86_64-w64-mingw32-ranlib)

if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "x86_64-w64-mingw32-gcc not found. Install mingw-w64: sudo apt install mingw-w64")
endif()

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "-Wl,--no-undefined -static-libgcc -static-libstdc++")

# Set find paths
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set minimum required CMake version.
cmake_minimum_required(VERSION 3.7)

# Set package name and version.
set(CPACK_PACKAGE_NAME "battleship")
set(CPACK_PACKAGE_VERSION "1.0.0")

# Use MinGW to compile for Windows.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32-g++)

# Register all source files.
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
file(GLOB_RECURSE sources src/*.cc src/*.h)

# Add executable from all source files.
add_executable(battleship.exe ${sources})

# Provide all warning output on compile.
target_compile_options(battleship.exe PUBLIC -Wall -Wno-unknown-pragmas)

# Link Windows libraries.
target_link_libraries(battleship.exe wsock32 ws2_32 iphlpapi x64/wpcap)

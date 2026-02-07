cmake_minimum_required(VERSION 3.16)
project(MyRaylibProject C)

set(CMAKE_C_STANDARD 11)

# Point to raylib source
set(RAYLIB_PATH "C:/raylib/raylib")
include_directories(${RAYLIB_PATH}/src)

# Build raylib from source
add_subdirectory(${RAYLIB_PATH} ${CMAKE_BINARY_DIR}/raylib_build)

add_executable(MyRaylibProject main.c)
add_executable(MapEditor MapEditor.c)

# Link against raylib
target_link_libraries(MyRaylibProject raylib)
target_link_libraries(MapEditor raylib)

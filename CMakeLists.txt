cmake_minimum_required(VERSION 3.10)
project(2dge)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
add_compile_options(-Wall -Wfatal-errors)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Find Lua package
find_package(Lua 5.4 REQUIRED)

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/libs
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIR}
        "/opt/homebrew/include"
)

# Collect source files
file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/Game/*.cpp"
        "src/Logger/*.cpp"
        "src/ECS/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${LUA_LIBRARIES}
)

# Custom targets equivalent to .Makefile's run and clean
add_custom_target(run
        COMMAND ${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Note: clean is already provided by CMake with 'make clean'
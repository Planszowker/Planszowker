# Set project settings
cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME PLANSZOWKER)

project(${PROJECT_NAME} VERSION 0.0.0)

set(SFML_DIR CACHE PATH "Path to SFML")

# Set compiler specific settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

find_package(SFML 2.5.1 COMPONENTS graphics network window system REQUIRED)

include_directories(common)

add_subdirectory(planszowker_server)
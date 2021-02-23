cmake_minimum_required(VERSION 3.12)

project(Planszowker VERSION 0.0
                    LANGUAGES CXX)

include (cmake/search_paths.cmake)

set(SFML_STATIC_LIBRARIES TRUE)

# Set compiler specific settings
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5.1 COMPONENTS network graphics window system REQUIRED)

include_directories(common)

add_subdirectory(common/Utils)
add_subdirectory(common/Network)
add_subdirectory(planszowker_server)
add_subdirectory(planszowker_client)
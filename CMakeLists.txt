cmake_minimum_required(VERSION 3.20)

project(Planszowker VERSION 0.0
                    LANGUAGES CXX)

set(SFML_STATIC_LIBRARIES TRUE)

# Set compiler specific settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5.1 COMPONENTS network graphics window system REQUIRED)
find_package(Sol2 REQUIRED)
find_package(Lua REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${LUA_INCLUDE_DIR})

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

include_directories(libs)

add_subdirectory(libs/Utils/ClientInfo)
add_subdirectory(libs/Utils/ErrorHandler)
add_subdirectory(libs/Utils/Logger)
add_subdirectory(libs/Utils/CompilerUtils)
add_subdirectory(libs/Utils/TimeMeasurement)
add_subdirectory(libs/Utils/Rng)
add_subdirectory(libs/Network/NetworkHandler)
add_subdirectory(libs/Games)
add_subdirectory(libs/GamesClient)
add_subdirectory(libs/GamesServer)
add_subdirectory(PlanszowkerServer)
add_subdirectory(PlanszowkerClient)
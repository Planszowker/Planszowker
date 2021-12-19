cmake_minimum_required(VERSION 3.12)

project(Planszowker VERSION 0.1
                    LANGUAGES CXX)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

# Set compiler specific settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

find_package(SFML 2.5.1 COMPONENTS network graphics window system REQUIRED)
find_package(SFGUI REQUIRED)

include_directories(common)

add_subdirectory(common/Utils/ClientInfo)
add_subdirectory(common/Utils/ErrorHandler)
add_subdirectory(common/Utils/Logger)
add_subdirectory(common/Utils/CompilerUtils)
add_subdirectory(common/Utils/TimeMeasurement)
add_subdirectory(common/Utils/Rng)
add_subdirectory(common/Network/NetworkHandler)
add_subdirectory(common/Games)
add_subdirectory(common/GameUtils/GameWindow)
add_subdirectory(PlanszowkerServer)
add_subdirectory(PlanszowkerClient)
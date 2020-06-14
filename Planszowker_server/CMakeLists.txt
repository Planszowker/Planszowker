# Set project settings
cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME PLANSZOWKER_SERVER)

project(${PROJECT_NAME})


# Set variable where binary will be put
set(BINARY_OUTPUT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_FOLDER})

# Set compiler specific settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")


# Set SFML directories
set(SFML_INCLUDE_DIR "C:/SFML/include")
set(SFML_LIBRARY_DIR "C:/SFML/lib")
set(SFML_DIR "C:/SFML/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE) # For static linking


# Include project specific include folders
include_directories(inc)
include_directories(../common/inc)


# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics network window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
link_directories(SFML_LIBRARY_DIR) # Tell compiler where libraries are


# Add sources
add_executable(${PROJECT_NAME}
        inc/Client.h
        src/Client.cpp
        ../common/src/ErrorLogger.cpp
        src/main.cpp)


# Link project with SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-network sfml-window sfml-system ${SFML_DEPENDENCIES})
# Set project settings
cmake_minimum_required(VERSION 3.8)
set(PROJECT_NAME MODBUS_ASCII_SLAVE)

project(${PROJECT_NAME} VERSION 0.0)

# Set variable where binary will be put
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    file(MAKE_DIRECTORY bin)
endif()
set(BINARY_OUTPUT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_FOLDER})

# Set compiler specific settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")


# Set SFML directories
if(WIN32)
    #set(SFML_INCLUDE_DIR "/usr/lib/include")
    #set(SFML_LIBRARY_DIR "/usr/lib")
    #set(SFML_DIR "/usr/lib/")
elseif(UNIX)
    set(SFML_INCLUDE_DIR "/usr/lib/include")
    set(SFML_LIBRARY_DIR "/usr/lib")
    set(SFML_DIR "/usr/lib/")
endif()
#set(SFML_STATIC_LIBRARIES TRUE) # For static linking


# Include project specific include folders
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sources)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/)


# Configure project version
configure_file(sources/Config/Config.h.in Config/Config.h)


# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics network window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
link_directories(SFML_LIBRARY_DIR) # Tell compiler where libraries are


# Add sources
add_executable(${PROJECT_NAME}
        sources/main.cpp
        sources/Core/Core.cpp
        sources/States/StatesHandler.cpp
        )


# Set custom command for build number
# https://github.com/nandee95/Auto_Build_Number_Increment
add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND}
        -DLANGUAGE:STRING="C"
        -DHEADER_DIR:PATH="${CMAKE_SOURCE_DIR}/inc"
        -DCACHE_DIR:PATH="${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_number.cmake"
)

# Link project with SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-network sfml-window sfml-system ${SFML_DEPENDENCIES})


# Allow Config.h file to be included in project
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
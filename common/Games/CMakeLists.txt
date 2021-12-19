set(LIB_NAME Games)

project(${LIB_NAME})

set(SOURCES
        GraphicalView.cpp
    )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(
        ${LIB_NAME}

        PUBLIC NetworkHandler
        PRIVATE GameWindow

        PUBLIC debug sfml-network-d
        PUBLIC debug sfml-graphics-d
        PUBLIC debug sfml-window-d
        PUBLIC debug sfml-system-d
        PUBLIC debug SFGUI-d

        PUBLIC optimized sfml-network
        PUBLIC optimized sfml-graphics
        PUBLIC debug sfml-window
        PUBLIC debug sfml-system
        PUBLIC optimized SFGUI
    )

target_include_directories(
        ${LIB_NAME}

        PUBLIC headers

        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE headers/${LIB_NAME}
    )

add_subdirectory(DiceRoller)
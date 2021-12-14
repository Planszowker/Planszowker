set(LIB_NAME Games)

project(${LIB_NAME})

set(SOURCES
      GameCreator.cpp
      GraphicalView.cpp
    )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler

                        PUBLIC debug sfml-graphics-d
                        PUBLIC debug sfml-window-d
                        PUBLIC debug sfml-system-d
                        PUBLIC debug SFGUI-d

                        PUBLIC optimized sfml-graphics
                        PUBLIC debug sfml-window
                        PUBLIC debug sfml-system
                        PUBLIC optimized SFGUI
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/Games
                           )

add_subdirectory(DiceRoller)
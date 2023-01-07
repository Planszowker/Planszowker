set(LIB_NAME GamesClient)

set(SOURCES
      Controller.cpp
      GraphicalView.cpp
      FontManager.cpp
    )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC Logger
                        PUBLIC ErrorHandler
                        PUBLIC Games
                        PUBLIC AssetsManager

                        sfml-graphics sfml-network sfml-system sfml-window

                        PRIVATE ImGui-SFML::ImGui-SFML
                      )

target_include_directories(${LIB_NAME}
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PUBLIC headers

                             PRIVATE headers/${LIB_NAME}
                           )

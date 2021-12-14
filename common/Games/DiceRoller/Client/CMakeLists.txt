set(LIB_NAME DiceRollerClient)

set(SOURCES
        GraphicalView.cpp
        Controller.cpp
        ViewLogic.cpp
    )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC Logger
                        PUBLIC ErrorHandler
                        PUBLIC Games
                        PUBLIC DiceRoller

                        PUBLIC ${SFML_LIBRARIES}
                        PUBLIC ${SFML_DEPENDENCIES}
                      )

target_include_directories(${LIB_NAME}
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PUBLIC headers

                             PRIVATE headers/DiceRoller
                           )

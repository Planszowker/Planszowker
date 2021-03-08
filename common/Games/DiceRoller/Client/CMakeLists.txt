set(LIB_NAME DiceRollerClient)

set(SOURCES
      ConsoleView.cpp
      Controller.cpp
      ViewLogic.cpp
    )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PRIVATE Logger
                        PRIVATE ErrorHandler
                        INTERFACE Games
                      )

target_include_directories(${LIB_NAME}
                             PRIVATE ${Games_SOURCE_DIR}/headers
                             PRIVATE ${DiceRoller_SOURCE_DIR}/headers
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PUBLIC headers

                             PRIVATE headers/DiceRoller
                           )

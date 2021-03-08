set(LIB_NAME DiceRollerServer)

set(SOURCES
      ServerHandler.cpp
      ServerLogic.cpp
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                             PRIVATE ${Games_SOURCE_DIR}/headers
                             PRIVATE ${DiceRoller_SOURCE_DIR}/headers
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}

                             PRIVATE headers/DiceRoller

                             PUBLIC headers
                           )

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler
                        INTERFACE Games
                      )

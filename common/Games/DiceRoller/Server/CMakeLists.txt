set(LIB_NAME DiceRollerServer)

set(SOURCES
      ServerHandler.cpp
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/DiceRoller

                             PUBLIC headers
                           )

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler
                        PUBLIC Games
                        PUBLIC DiceRoller
                      )

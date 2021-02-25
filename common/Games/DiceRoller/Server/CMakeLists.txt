set(LIB_NAME DiceRollerServer)

set(SOURCES
      DiceRollerServerLogic.cpp
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                             PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                             PUBLIC headers

                             PRIVATE headers/DiceRoller
                           )

target_link_libraries(${LIB_NAME}
                        PRIVATE NetworkHandler
                      )

set(LIB_NAME DiceRollerClient)

set(SOURCES
      DiceRollerConsoleView.cpp
    )

include_directories(${Games_SOURCE_DIR}/headers) # Include Games' common headers. TODO: Check for any better method ;)

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                             PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                             PUBLIC headers

                             PRIVATE headers/DiceRoller
                           )

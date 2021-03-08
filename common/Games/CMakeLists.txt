set(LIB_NAME Games)

project(${LIB_NAME})

set(SOURCES
      GameCreator.cpp
    )

add_library(${LIB_NAME} STATIC ${SOURCES})

add_subdirectory(DiceRoller)

target_link_libraries(${LIB_NAME})

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/Games
                           )
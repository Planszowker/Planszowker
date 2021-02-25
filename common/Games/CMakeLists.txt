set(LIB_NAME Games)

project(${LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

add_subdirectory(DiceRoller)

target_link_libraries(${LIB_NAME}
                      INTERFACE NetworkHandler
                      INTERFACE NetworkQuery
                      )

target_include_directories(${LIB_NAME}
                             INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                             INTERFACE headers/Games
                           )
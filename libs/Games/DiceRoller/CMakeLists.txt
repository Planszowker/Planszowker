set(LIB_NAME DiceRoller)

project(${LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

add_subdirectory(Client)

target_link_libraries(${LIB_NAME}
                        INTERFACE DiceRollerClient
                      )

target_include_directories(${LIB_NAME}
                             INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                             INTERFACE headers
                           )

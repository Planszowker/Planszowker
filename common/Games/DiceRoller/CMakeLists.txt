set(LIB_NAME DiceRoller)

add_library(${LIB_NAME} INTERFACE)

add_subdirectory(Client)
add_subdirectory(Server)

target_include_directories(${LIB_NAME}
                             INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                             INTERFACE headers

                             INTERFACE headers/${LIB_NAME}
                           )

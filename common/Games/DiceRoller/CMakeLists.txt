set(LIB_NAME DiceRoller)

add_library(${LIB_NAME} INTERFACE)

add_subdirectory(Client)
add_subdirectory(Server)

target_include_directories(${LIB_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           PUBLIC headers

                           PRIVATE headers/${LIB_NAME}
                           )

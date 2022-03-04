set(LIB_NAME Games)

project(${LIB_NAME})

set(SOURCES
        GraphicalView.cpp
        Objects.cpp
        )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler

                        PUBLIC lua5.3

                        PUBLIC sfml-network
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/Games
                           )
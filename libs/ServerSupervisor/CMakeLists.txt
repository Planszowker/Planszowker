set(LIB_NAME ServerSupervisor)

project(${LIB_NAME})

set(SOURCES
        ServerSupervisor.cpp
        Command.cpp
   )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC EasyLogging
                        PUBLIC zipios
                        PUBLIC PlametaParser
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/${LIB_NAME}
                           )

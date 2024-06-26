set(LIB_NAME Supervisor)

project(${LIB_NAME})

set(SOURCES
        Supervisor.cpp
        Command.cpp
        GamesInfoExtractor.cpp
        Lobby.cpp
        Lobbies.cpp
   )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC EasyLogging
                        PUBLIC PlametaParser
                        PUBLIC NetworkHandler
                        PUBLIC ThreadSafeQueue
                        PUBLIC GamesServer
                        PUBLIC TickThread

                        PRIVATE ${ZIPLIB} ${ZIPLIB_BZIP2} ${ZIPLIB_LZMA} ${ZIPLIB_ZLIB}
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/${LIB_NAME}
                           )

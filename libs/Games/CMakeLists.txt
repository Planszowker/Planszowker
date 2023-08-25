set(LIB_NAME Games)

project(${LIB_NAME})

set(SOURCES
        CommObjects.cpp
        GameWindow.cpp
        GamesMetaInfo.cpp
        BoardParser.cpp
        States/GameChoosingState.cpp
        States/GameState.cpp
        States/GameLobbyState.cpp

        Callbacks/ICallbacks.cpp
        Callbacks/GameChoosingCallbacks.cpp
        Callbacks/GameLobbyCallbacks.cpp
        Callbacks/GameCallbacks.cpp

        Objects/ActionButton.cpp
        Objects/DestinationPoint.cpp
        Objects/Entity.cpp
        Objects/Tile.cpp
        )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler
                        PRIVATE GamesClient
                        PUBLIC PlametaParser
                        PUBLIC ThreadSafeQueue
                        PUBLIC Supervisor

                        PUBLIC lua

                        PUBLIC sfml-network
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/Games
                           )

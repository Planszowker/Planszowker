set(LIB_NAME AssetsManager)

project(${LIB_NAME})

set(SOURCES
        AssetsTransmitter.cpp
        AssetsReceiver.cpp
   )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler
                        PUBLIC Games
                        PRIVATE GamesServer

                        PRIVATE zipios
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/${LIB_NAME}
                           )

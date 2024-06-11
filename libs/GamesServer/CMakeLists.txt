set(LIB_NAME GamesServer)

project(${LIB_NAME})

set(SOURCES
        ServerHandler.cpp
        Logic.cpp
        GamesHandler.cpp
   )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME}
                        PUBLIC NetworkHandler
                        PUBLIC ThreadSafeQueue
                        PUBLIC Games
                        PUBLIC Rng
                        PUBLIC TimeMeasurement
                        PUBLIC CompilerUtils
                        PUBLIC AssetsManager

                        PUBLIC lua

                        PRIVATE ZipLib
                      )

target_include_directories(${LIB_NAME}
                             PUBLIC headers

                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/${LIB_NAME}
                           )

set(LIB_NAME NetworkHandler)

set(SOURCES
        ServerPacketHandler.cpp
        ClientPacketHandler.cpp
   )

add_library(${LIB_NAME} STATIC ${SOURCES})

target_include_directories(${LIB_NAME}
                           PRIVATE ${Games_SOURCE_DIR}/headers
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           PUBLIC headers

                           PRIVATE headers/${LIB_NAME}
                           )

target_link_libraries(${LIB_NAME}
                      PUBLIC ClientInfo
                      PUBLIC ErrorHandler
                      PUBLIC Logger
                      PUBLIC TimeMeasurement
                      PUBLIC CompilerUtils
                      PUBLIC Games

                      PUBLIC sfml-network
        )
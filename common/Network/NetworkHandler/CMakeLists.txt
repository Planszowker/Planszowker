set(LIB_NAME NetworkHandler)

set(SOURCES
    NetworkHandler.cpp
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           PUBLIC headers

                           PRIVATE headers/${LIB_NAME}
                           )

target_link_libraries(${LIB_NAME}
                      PRIVATE ClientInfo
                      PRIVATE ErrorHandler
                      PRIVATE Logger
                      PRIVATE NetworkQuery

                      PUBLIC sfml-network)
set(LIB_NAME ClientInfo)

set(SOURCES
    ClientInfo.cpp
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                           PRIVATE headers/ClientInfo
                           PUBLIC headers
                           )
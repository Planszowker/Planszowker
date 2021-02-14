set(LIB_NAME ErrorLogger)

set(SOURCES
    ErrorLogger.cpp
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                           PRIVATE headers/ErrorLogger
                           PUBLIC headers
                           )
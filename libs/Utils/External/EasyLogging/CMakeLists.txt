set(LIB_NAME EasyLogging)

set(SOURCES
    easylogging++.cc
    )

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(${LIB_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           )
set(LIB_NAME CompilerUtils)

set(SOURCES)

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           INTERFACE headers

                           INTERFACE headers/${LIB_NAME}
                           )
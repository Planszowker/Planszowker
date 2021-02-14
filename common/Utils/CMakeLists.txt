set(LIB_NAME Utils)

project(${LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

target_link_libraries(${LIB_NAME}
                      PUBLIC ErrorLogger
                      PUBLIC ClientInfo
                      )
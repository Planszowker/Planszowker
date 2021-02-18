set(LIB_NAME Utils)

project(${LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

add_subdirectory(ClientInfo)
add_subdirectory(ErrorHandler)

target_link_libraries(${LIB_NAME}
                      INTERFACE ErrorHandler
                      INTERFACE ClientInfo
                      )
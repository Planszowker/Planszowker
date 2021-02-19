set(LIB_NAME Network)

project(${LIB_NAME})

add_library(${LIB_NAME} INTERFACE)

add_subdirectory(NetworkHandler)
add_subdirectory(NetworkQuery)

target_link_libraries(${LIB_NAME}
                      INTERFACE NetworkHandler
                      INTERFACE NetworkQuery
                      )
set(LIB_NAME ThreadSafeQueue)

add_library(${LIB_NAME} INTERFACE)

target_link_libraries(${LIB_NAME}
                      INTERFACE Games
                      )

target_include_directories(${LIB_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           INTERFACE headers
                           )
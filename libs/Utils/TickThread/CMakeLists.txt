set(LIB_NAME TickThread)

add_library(${LIB_NAME} STATIC TickThread.cpp)

target_link_libraries(
    ${LIB_NAME}
        PRIVATE Games
        PUBLIC ErrorHandler
)

target_include_directories(${LIB_NAME}
                            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                            PUBLIC headers

                            PRIVATE headers/${LIB_NAME}
                           )
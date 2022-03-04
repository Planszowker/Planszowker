set(EXEC_NAME PlanszowkerClient)

set(SOURCES
    main.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE ErrorHandler
                      PRIVATE GamesClient

                      sfml-graphics
                      sfml-network

                      PUBLIC nlohmann_json::nlohmann_json

                      -static-libgcc
                      -static-libstdc++
                      )

target_include_directories(${EXEC_NAME}
                           PRIVATE {CMAKE_CURRENT_SOURCE_DIR}
                           )

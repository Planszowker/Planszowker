set(EXEC_NAME planszowker_client)

set(SOURCES
    main.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE Utils
                      PRIVATE Network

                      sfml-graphics
                      sfml-network)

target_include_directories(${EXEC_NAME}
                           PRIVATE {CMAKE_CURRENT_SOURCE_DIR})
                           #PRIVATE headers/planszowker_client
                           #PRIVATE headers)
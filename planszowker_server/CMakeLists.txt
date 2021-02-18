set(EXEC_NAME planszowker_server)

set(SOURCES
    main.cpp
    NetworkHandler.cpp
    Server.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE Utils

                      sfml-graphics
                      sfml-network)

target_include_directories(${EXEC_NAME}
                           PRIVATE {CMAKE_CURRENT_SOURCE_DIR}
                           PRIVATE headers/planszowker_server
                           PRIVATE headers)
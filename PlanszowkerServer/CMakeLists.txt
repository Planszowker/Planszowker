set(EXEC_NAME PlanszowkerServer)

set(SOURCES
    main.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE ErrorHandler
                      PRIVATE DiceRoller

                      sfml-graphics
                      sfml-network
                      )

target_include_directories(${EXEC_NAME}
                             PRIVATE {CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/PlanszowkerServer
                             PRIVATE headers
                           )

set(EXEC_NAME PlanszowkerClient)

set(SOURCES
    main.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE ErrorHandler
                      PRIVATE DiceRoller

                      PUBLIC debug sfml-graphics-d
                      PUBLIC optimized sfml-graphics
                      )

target_include_directories(${EXEC_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

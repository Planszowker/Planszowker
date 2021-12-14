set(EXEC_NAME PlanszowkerClient)

set(SOURCES
    main.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE ErrorHandler
                      PRIVATE DiceRoller

                      PUBLIC ${SFML_LIBRARIES}
                      PUBLIC ${SFML_DEPENDENCIES}
                      )

target_include_directories(${EXEC_NAME}
                           PUBLIC ${SFML_INCLUDE_DIR}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

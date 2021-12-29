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

# Testing
find_package(SFML 2.5.1 COMPONENTS network graphics window system REQUIRED)
find_package(SFGUI REQUIRED)
add_executable(TEST sfgui_test.cpp)
target_link_libraries(TEST sfml-graphics-d sfml-system-d sfml-window-d SFGUI-d)
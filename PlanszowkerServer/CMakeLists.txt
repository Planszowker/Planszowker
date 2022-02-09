set(EXEC_NAME PlanszowkerServer)

set(
        SOURCES
        main.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})

add_executable(testingLua testingLUA.cpp)
target_link_libraries(testingLua PRIVATE lua5.3)

add_executable(testingZipios testingZipios.cpp)
target_link_libraries(testingZipios PRIVATE zipios)

target_link_libraries(${EXEC_NAME}
                      PRIVATE ErrorHandler
                      PRIVATE GamesServer

                      sfml-graphics
                      sfml-network

                      PUBLIC nlohmann_json::nlohmann_json

                      -static-libgcc
                      -static-libstdc++
                      )

target_include_directories(${EXEC_NAME}
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                             PRIVATE headers/PlanszowkerServer
                             PRIVATE headers
                           )

file(COPY lua-scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

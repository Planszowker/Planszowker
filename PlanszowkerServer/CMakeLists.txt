set(EXEC_NAME PlanszowkerServer)

set(SOURCES
        main.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME}
                      PRIVATE ErrorHandler
                      PRIVATE GamesServer
                      PRIVATE EasyLogging
                      PRIVATE Supervisor

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
# Copy config
file(COPY config.plameta DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copy core lua scripts, except games
file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        PATTERN "scripts/games/**" EXCLUDE)

add_custom_target(PackGames ALL
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/../python/pack_games.py ${CMAKE_CURRENT_BINARY_DIR}/scripts/games
        COMMENT "Packing games into .plagame files"
        DEPEND always_rebuild
        VERBATIM
        )

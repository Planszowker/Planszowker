include(${CMAKE_SOURCE_DIR}/cmake/search_paths)

set(EXEC_NAME planszowker_server)

set(SOURCES
    main.cpp
    )

add_executable(${EXEC_NAME} ${SOURCES})

find_library(Utils)

target_link_libraries(Utils)
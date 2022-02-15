set(EXEC_NAME FunctionalTests)

add_executable(LuaTest LuaTest.cpp)
target_link_libraries(LuaTest PRIVATE lua5.3)

add_executable(ZipiosTest ZipiosTest.cpp)
target_link_libraries(ZipiosTest PRIVATE zipios)

add_executable(AssetsTransferClient AssetsTransferClient.cpp)
target_link_libraries(AssetsTransferClient PRIVATE sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d)

add_executable(AssetsTransferServer AssetsTransferServer.cpp)
target_link_libraries(AssetsTransferServer PRIVATE sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d)
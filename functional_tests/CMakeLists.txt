set(EXEC_NAME FunctionalTests)

add_executable(LuaTest LuaTest.cpp)
target_link_libraries(LuaTest PRIVATE lua5.3)

add_executable(ZipiosTest ZipiosTest.cpp)
target_link_libraries(ZipiosTest PRIVATE zipios)

add_executable(AssetsTransferClient AssetsTransferClient.cpp)
target_link_libraries(AssetsTransferClient PUBLIC sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d)

add_executable(AssetsTransferServer AssetsTransferServer.cpp)
target_link_libraries(AssetsTransferServer PUBLIC sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d)

add_executable(ImGuiTest ImGuiTest.cpp)
target_link_libraries(ImGuiTest PUBLIC sfml-graphics-d sfml-network-d sfml-system-d sfml-window-d ImGui-SFML::ImGui-SFML)
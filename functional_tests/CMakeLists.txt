set(EXEC_NAME FunctionalTests)

add_executable(LuaTest LuaTest.cpp)
target_link_libraries(LuaTest PRIVATE lua)

add_executable(ZipLibTest ZipLibTest.cpp)
target_link_libraries(ZipLibTest PRIVATE ZipLib
        sfml-graphics sfml-network sfml-system sfml-window Base64)

add_executable(ImGuiTest ImGuiTest.cpp)
target_link_libraries(ImGuiTest PUBLIC sfml-graphics sfml-network sfml-system sfml-window ImGui-SFML::ImGui-SFML)
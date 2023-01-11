set(EXEC_NAME FunctionalTests)

add_executable(LuaTest LuaTest.cpp)
target_link_libraries(LuaTest PRIVATE lua)

add_executable(ZipLibTest ZipLibTest.cpp)
target_link_libraries(ZipLibTest PRIVATE ${ZIPLIB_BZIP2} ${ZIPLIB_LZMA} ${ZIPLIB_ZLIB} ${ZIPLIB})

add_executable(AssetsTransferClient AssetsTransferClient.cpp)
target_link_libraries(AssetsTransferClient PUBLIC sfml-graphics sfml-network sfml-system sfml-window)

add_executable(AssetsTransferServer AssetsTransferServer.cpp)
target_link_libraries(AssetsTransferServer PUBLIC sfml-graphics sfml-network sfml-system sfml-window)

add_executable(ImGuiTest ImGuiTest.cpp)
target_link_libraries(ImGuiTest PUBLIC sfml-graphics sfml-network sfml-system sfml-window ImGui-SFML::ImGui-SFML)
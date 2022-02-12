set(EXEC_NAME FunctionalTests)

add_executable(LuaTest LuaTest.cpp)
target_link_libraries(LuaTest PRIVATE lua5.3)

add_executable(ZipiosTest ZipiosTest.cpp)
target_link_libraries(ZipiosTest PRIVATE zipios)

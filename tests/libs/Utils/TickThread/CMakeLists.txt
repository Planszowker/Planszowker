add_executable(
        TickThreadTest
        TickThreadTest.cpp
)
target_link_libraries(
        TickThreadTest
        PRIVATE TickThread
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(TickThreadTest)
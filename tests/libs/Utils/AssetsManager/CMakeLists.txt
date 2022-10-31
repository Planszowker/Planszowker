add_executable(
        AssetsReceiverTest
        AssetsReceiverTest.cpp
)
target_link_libraries(
        AssetsReceiverTest
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(AssetsReceiverTest)
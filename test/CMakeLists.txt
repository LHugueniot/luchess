include(CTest)
enable_testing()


# Create test executable
add_executable(luchess_core_tests ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

# Link internal module libs
target_link_libraries(
    luchess_core_tests

    PUBLIC
    LuChessCore
)


# Gtest setup
find_package(GTest CONFIG REQUIRED)
target_link_libraries(luchess_core_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(
    NAME luchess_core_tests
    COMMAND test/luchess_core_tests
)


# Add GoogleTest using vcpkg
find_package(GTest CONFIG REQUIRED)

# Add the test executable
add_executable(test_bv test_bv.cpp)

target_link_libraries(test_bv
    GTest::gtest_main
    random_constraint_solver
)

include(GoogleTest)
gtest_discover_tests(test_bv)

cmake_minimum_required(VERSION 2.8.6)

add_definitions(-std=c++0x)

set(CMAKE_BUILD_TYPE Release)

add_executable(minvm main.cpp)

include_directories(core)
include_directories(impl)

add_subdirectory(core)
add_subdirectory(impl)
add_subdirectory(test)

target_link_libraries(minvm core)
target_link_libraries(minvm impl)

enable_testing()
add_test(tests
    test/test_runner)

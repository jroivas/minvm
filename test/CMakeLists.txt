
include_directories(../core)

add_executable(test_runner
    runner.cpp
    framework.cpp
    regs.cpp
    )
target_link_libraries(test_runner core)

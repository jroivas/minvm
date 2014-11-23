
include_directories(../core)

add_executable(test_runner
    runner.cpp
    framework.cpp
    regs.cpp
    vm.cpp
    )
target_link_libraries(test_runner core)
target_link_libraries(test_runner impl)

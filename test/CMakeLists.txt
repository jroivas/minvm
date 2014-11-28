
include_directories(../core)

add_executable(test_runner
    runner.cpp
    framework.cpp
    regs.cpp
    vm.cpp
    random.cpp
    mov.cpp
    ints.cpp
    )
target_link_libraries(test_runner core)
target_link_libraries(test_runner impl)

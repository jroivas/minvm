add_definitions(-std=c++0x)

set(CMAKE_BUILD_TYPE Release)

add_executable(minvm
    main.cpp
    core/regs.cpp
    core/vm.cpp
    impl/ints.cpp
    impl/strs.cpp
    impl/random.cpp
    impl/jump.cpp)

include_directories(core)

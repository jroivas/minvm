cmake_minimum_required(VERSION 2.8.6)

add_definitions(-std=c++0x)

set(CMAKE_BUILD_TYPE Release)

add_executable(minvm main.cpp)

include_directories(core)
include_directories(impl)
include_directories(.)

add_subdirectory(core)
add_subdirectory(impl)
add_subdirectory(test)

target_link_libraries(minvm core)
target_link_libraries(minvm impl)

enable_testing()
add_test(tests
    test/test_runner)

add_custom_target(unittest ALL
    COMMAND python -mdoctest "${CMAKE_CURRENT_LIST_DIR}/compiler/assemble.py"
    DEPENDS compiler/assemble.py)

set(assembly_tests loop_simple jump_forward bench noexit info_heap load_mem)
set(test_targets "")

foreach(atest ${assembly_tests})
add_custom_target(functional_test_${atest} ALL
    COMMAND "${CMAKE_CURRENT_LIST_DIR}/compiler/assemble.py" --quiet "${CMAKE_CURRENT_LIST_DIR}/examples/${atest}.asm" ${atest}.bin
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/minvm" ${atest}.bin > ${atest}.test 2>&1 || /bin/true
    COMMAND diff -u "${CMAKE_CURRENT_LIST_DIR}/test/outputs/${atest}.out" ${atest}.test
    DEPENDS minvm "${CMAKE_CURRENT_LIST_DIR}/examples/${atest}.asm"
    )
set(test_targets ${test_targets} ${atest}.test)
endforeach()

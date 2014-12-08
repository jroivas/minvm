add_library(impl STATIC
    nopstop.cpp
    ints.cpp
    strs.cpp
    random.cpp
    jump.cpp
    heap.cpp
    mov.cpp)

include_directories(.)
include_directories(..)

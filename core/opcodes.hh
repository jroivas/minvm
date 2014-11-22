#pragma once

#include <cstdint>

namespace core
{

enum class Opcode : uint8_t
{
    NOP,

    LOAD_INT,
    STORE_INT8,
    STORE_INT16,
    STORE_INT32,
    STORE_INT64,

    LOAD_STR,
    STORE_STR,

    INC_INT,
    DEC_INT,
    ADD_INT,
    SUB_INT,
    MUL_INT,
    DIV_INT,
    MOD_INT,

    PRINT_INT,
    PRINT_FLOAT,
    PRINT_STR,

    STOP
};

}

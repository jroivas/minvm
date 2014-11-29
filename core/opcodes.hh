#pragma once

#include <cstdint>

namespace core
{

enum class Info : uint8_t
{
    Info = 0,

    Ticks,

    HeapSize,
    HeapStart,
};

enum class Opcode : uint8_t
{
    NOP = 0,

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

    RANDOM,

    JMP8,
    JMP16,
    JMP32,
    JMP64,
    JMP_INT,

    JMP_LE8,
    JMP_LE16,
    JMP_LE32,
    JMP_LE64,
    JMP_LE_INT,

    MOV,
    HEAP,
    INFO,

    STOP = 255
};

}

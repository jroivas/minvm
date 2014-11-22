#pragma once

#include "vm.hh"

class ASM1
{
public:
    ASM1(core::VM *vm);

    static bool store_int8(core::VM *vm);
    static bool store_int32(core::VM *vm);
    static bool store_int64(core::VM *vm);

    static bool add_int(core::VM *vm);
    static bool print_int(core::VM *vm);

    static bool store_str(core::VM *vm);
    static bool print_str(core::VM *vm);
};

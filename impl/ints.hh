#pragma once

#include "vm.hh"

namespace impl
{

class Ints
{
public:
    Ints(core::VM *vm);

    static bool store_int8(core::VM *vm);
    static bool store_int32(core::VM *vm);
    static bool store_int64(core::VM *vm);

    static bool add_int(core::VM *vm);
    static bool print_int(core::VM *vm);
};

}

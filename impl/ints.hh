#pragma once

#include "vm.hh"

namespace impl
{

class Ints
{
public:
    Ints(core::VM *vm);

private:
    static bool load_int8(core::VM *vm);
    static bool load_int16(core::VM *vm);
    static bool load_int32(core::VM *vm);
    static bool load_int64(core::VM *vm);

    static bool load_int_mem(core::VM *vm);

    static bool inc_int(core::VM *vm);
    static bool dec_int(core::VM *vm);
    static bool add_int(core::VM *vm);
    static bool sub_int(core::VM *vm);

    static bool mul_int(core::VM *vm);
    static bool div_int(core::VM *vm);
    static bool mod_int(core::VM *vm);

    static bool print_int(core::VM *vm);
};

}

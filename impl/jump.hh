#pragma once

#include "vm.hh"

namespace impl
{

class Jump
{
public:
    Jump(core::VM *vm);

    static bool jump8(core::VM *vm);
    static bool jump16(core::VM *vm);
    static bool jump32(core::VM *vm);
    static bool jump64(core::VM *vm);
    static bool jump_int(core::VM *vm);

    static bool jump_le8(core::VM *vm);
    static bool jump_le16(core::VM *vm);
    static bool jump_le32(core::VM *vm);
    static bool jump_le64(core::VM *vm);
    static bool jump_le_int(core::VM *vm);

    static bool conditional(
        core::VM *vm, uint8_t algo, uint8_t val1, uint8_t val2);

    static void jump_conditional(
        core::VM *vm, uint64_t addr, bool cond);
};

}

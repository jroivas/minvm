#pragma once

#include "vm.hh"

namespace impl
{

class NopStop
{
public:
    NopStop(core::VM *vm);

private:
    static bool nop(core::VM *vm);
    static bool stop(core::VM *vm);
};

}

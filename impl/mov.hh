#pragma once

#include "vm.hh"
#include <random>

namespace impl
{

class Mov
{
public:
    Mov(core::VM *vm);

private:
    static bool mov(core::VM *vm);
};

}

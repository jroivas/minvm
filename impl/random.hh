#pragma once

#include "vm.hh"
#include <random>

namespace impl
{

class Random
{
public:
    Random(core::VM *vm);

private:
    static bool random(core::VM *vm);
};

}

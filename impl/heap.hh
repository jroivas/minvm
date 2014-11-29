#pragma once

#include "vm.hh"

namespace impl
{

class Heap
{
public:
    Heap(core::VM *vm);

private:
    static bool heap(core::VM *vm);
    static bool info(core::VM *vm);
};

}

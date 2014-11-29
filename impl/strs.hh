#pragma once

#include "vm.hh"

namespace impl
{

class Strs
{
public:
    Strs(core::VM *vm);

private:
    static bool load_str(core::VM *vm);
    static bool print_str(core::VM *vm);
};

}

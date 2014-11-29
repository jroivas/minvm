#include "strs.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;
using impl::Strs;

Strs::Strs(VM *vm)
{
    vm->opcode((uint32_t)Opcode::LOAD_STR, Strs::load_str);
    vm->opcode((uint32_t)Opcode::PRINT_STR, Strs::print_str);
}

bool Strs::load_str(core::VM *vm)
{
    if (vm->debug()) std::cerr << "LOAD_STR\n";
    std::string res = "";
    uint8_t reg = vm->fetch8();

    uint8_t val = vm->fetch8();
    while (val != 0) {
        res.push_back(val);
        val = vm->fetch8();
    }
    vm->regs().put_string(reg, res);

    return true;
}

bool Strs::print_str(core::VM *vm)
{
    if (vm->debug()) std::cerr << "PRINT_STR\n";
    std::cout << vm->regs().get_string(vm->fetch8());

    return true;
}

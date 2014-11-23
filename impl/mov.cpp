#include "mov.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;
using impl::Mov;

Mov::Mov(VM *vm)
{
    vm->opcode((uint32_t)Opcode::MOV, Mov::mov);
}

bool Mov::mov(core::VM *vm)
{
    if (vm->debug()) std::cerr << "MOV\n";

    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();

    vm->regs().copy(reg1, reg2);

    return true;
}

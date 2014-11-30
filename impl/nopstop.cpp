#include "nopstop.hh"

using core::VM;
using core::Opcode;
using impl::NopStop;

NopStop::NopStop(core::VM *vm)
{
    vm->opcode((uint32_t)Opcode::NOP, NopStop::nop);
    vm->opcode((uint32_t)Opcode::STOP, NopStop::stop);
}

bool NopStop::nop(core::VM *vm)
{
    return true;
}

bool NopStop::stop(core::VM *vm)
{
    return false;
}

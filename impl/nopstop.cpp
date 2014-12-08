#include "nopstop.hh"
#include "opcodes.hh"

using core::VM;
using impl::NopStop;
using impl::Opcode;

NopStop::NopStop(core::VM *vm)
{
    vm->opcode(Opcode::NOP(), NopStop::nop);
    vm->opcode(Opcode::STOP(), NopStop::stop);
}

bool NopStop::nop(core::VM *vm)
{
    return true;
}

bool NopStop::stop(core::VM *vm)
{
    return false;
}

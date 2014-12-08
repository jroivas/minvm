#include "random.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using impl::Opcode;
using impl::Random;

Random::Random(VM *vm)
{
    vm->opcode(Opcode::RANDOM(), Random::random);
}

bool Random::random(core::VM *vm)
{
    if (vm->debug()) std::cerr << "RANDOM\n";

    std::random_device m_rand;
    std::uniform_int_distribution<uint64_t> dist;

    vm->regs().put_int(
        vm->fetch8(),
        dist(m_rand));

    return true;
}

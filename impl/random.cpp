#include "random.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;
using impl::Random;

Random::Random(VM *vm)
{
    vm->opcode((uint32_t)Opcode::RANDOM, Random::random);
}

bool Random::random(core::VM *vm)
{
    if (vm->debug()) std::cerr << "RANDOM\n";

    std::random_device m_rand;
    std::uniform_int_distribution<uint64_t> dist;

    vm->regs().store_int(
        vm->fetch8(),
        dist(m_rand));

    return true;
}

#include "vm.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;


VM::VM() : m_debug(false)
{
    init();
}

VM::VM(uint8_t *mem) : m_mem(mem), m_debug(false)
{
    init();
}

void VM::init()
{
    for (uint32_t i = 0; i < 256; ++i) {
        m_opcodes[i] = VM::nop;
    }
    m_opcodes[(uint32_t)Opcode::STOP] = VM::stop;
}

void VM::load(uint8_t *mem)
{
    m_mem = mem;
    m_regs.reset_pc();
}

uint8_t VM::fetch8()
{
    uint8_t res = m_mem[m_regs.pc()];
    m_regs.next();
    return res;
}

Opcode VM::fetch()
{
    return (Opcode)fetch8();
}

bool VM::step()
{
    Opcode op = fetch();
    //std::cout << (int) op << "\n";
    return m_opcodes[(uint8_t)op](this);
}

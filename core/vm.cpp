#include "vm.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;


VM::VM() :
    m_mem(nullptr), m_size(0), m_debug(false)
{
    init();
}

VM::VM(uint8_t *mem, uint64_t size) :
    m_mem(mem), m_size(size), m_debug(false)
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

void VM::load(uint8_t *mem, uint64_t size)
{
    m_mem = mem;
    m_size = size;
    m_regs.pc_reset();
}

uint8_t VM::fetch8()
{
    uint64_t pos = m_regs.pc();
    if (pos >= m_size)
        throw std::string("Memory access out of bounds");
    if (m_mem == nullptr)
        throw std::string("Invalid memory");

    uint8_t res = m_mem[pos];
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

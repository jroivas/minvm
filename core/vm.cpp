#include "vm.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;


VM::VM() :
    m_mem(nullptr), m_size(0),
    m_heap_pos(0), m_ticks(0),
    m_debug(false)
{
    init();
}

VM::VM(uint8_t *mem, uint64_t size) :
    m_mem(mem), m_size(size),
    m_heap_pos(0), m_ticks(0),
    m_debug(false)
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
    ++m_ticks;
    //std::cout << (int) op << "\n";
    return m_opcodes[(uint8_t)op](this);
}

void VM::add_heap(uint64_t size)
{
    m_heap.emplace_back(m_heap_pos, size);
    m_heap_pos += size;
}

bool VM::is_heap(uint64_t pos) const
{
    for (auto &item : m_heap) {
        if (item.valid(pos)) {
            return true;
        }
    }
    return false;
}

uint8_t VM::get_heap(uint64_t pos) const
{
    for (auto &item : m_heap) {
        if (item.valid(pos)) {
            return item[pos];
        }
    }

    throw std::string("Invalid heap access");
}

void VM::set_heap(uint64_t pos, uint8_t val)
{
    for (auto &item : m_heap) {
        if (item.valid(pos)) {
            item[pos] = val;
            return;
        }
    }

    throw std::string("Invalid heap access");
}

core::Heap &VM::heap(uint64_t pos)
{
    for (auto &item : m_heap) {
        if (item.valid(pos)) {
            return item;
        }
    }

    throw std::string("Invalid heap access");
}

uint8_t VM::mem(uint64_t pos) const
{
/*
    if (pos >= m_size)
        throw std::string("Memory access out of bounds");
*/
    if (pos >= m_size)
        return get_heap(pos - m_size);

    return m_mem[pos];
}

#pragma once

#include <functional>

#include "regs.hh"
#include "opcodes.hh"
#include "heap.hh"

namespace core
{

class VM
{
public:
    VM();
    VM(uint8_t *mem, uint64_t size);

    inline void set_debug()
    {
        m_debug = true;
    }

    void load(uint8_t *mem, uint64_t size);
    Opcode fetch();
    uint8_t fetch8();

    bool step();
    inline void opcode(
        uint8_t num,
        std::function<bool (VM *)> func)
    {
        m_opcodes[num] = func;
    }

    inline std::function<bool (VM *)> get_opcode(uint8_t num)
    {
        return m_opcodes[num];
    }

    inline bool debug() const
    {
        return m_debug;
    }
    Registers &regs()
    {
        return m_regs;
    }
    void add_heap(uint64_t size);
    bool is_heap(uint64_t pos) const;
    uint8_t heap(uint64_t pos);

private:
    static bool nop(VM *)
    {
        return true;
    }
    static bool stop(VM *)
    {
        return false;
    }
    void init();

    std::function<bool (VM *)> m_opcodes[256];
    Registers m_regs;
    uint8_t *m_mem;
    uint64_t m_size;

    std::vector<Heap> m_heap;
    uint64_t m_heap_pos;

    bool m_debug;
};

}

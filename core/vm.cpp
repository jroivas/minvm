#include "vm.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;


#if 0
Operation::Operation(uint32_t val)
{
    for (int i = 0; i < 4; ++i) {
        data[i] = val & 0xFF;
        val >>= 8;
    }
}
#endif

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



#if 0

Operation VM::decode(uint32_t opcode)
{
    Operation tmp;
    for (unsigned int i = 0; i < 4 ; ++i)
        tmp.data[i] = m_mem[m_regs.pc() + i];
    m_regs.next();
    return tmp;
    //return Operation(opcode);
}

uint32_t VM::fetch()
{
    return 0;
/*
    uint32_t *tmp = (uint32_t*)m_mem;
    return tmp[m_regs.pc()];
*/
}

void VM::store(Operation &op)
{
    if (m_debug)
        std::cout << "STORE " << (int)op.opt[0] << " "<< (int)op.opt[1] << " "<< (int)op.opt[2]<< "\n";

    switch (op.opt[1]) {
        case 0:
            m_regs.store(op.opt[0], op.opt[2]);
            break;
        case 1:
            {
                std::string res;
                res.push_back(op.opt[2]);
                uint8_t val = m_mem[m_regs.pc()];
                while (val != 0) {
                    res.push_back(val);
                    m_regs.inc_pc();
                    val = m_mem[m_regs.pc()];
                }
                m_regs.inc_pc();
                //std::cout << res << "\n";
            }
            break;
        default:
            throw std::string("Invalid store!");
    }
/*
    uint64_t tmp = op.opt[1];
    tmp <<= 8;
    tmp |= op.opt[2];

    m_regs.store(op.opt[0], tmp);
*/
}

void VM::load(Operation &op)
{
    if (m_debug)
        std::cout << "LOAD " << (int)op.opt[0] << " "<< (int)op.opt[1] << " "<< (int)op.opt[2]<< "\n";
    uint64_t val = m_regs.load(op.opt[0]);
    uint64_t tmp = op.opt[1];
    tmp <<= 8;
    tmp |= op.opt[2];
    // FIXME store val to tmp
}

void VM::add(Operation &op)
{
    if (m_debug)
        std::cerr << "ADD " << (int)op.opt[0] << " "<< (int)op.opt[1] << " "<< (int)op.opt[2]<< "\n";

    uint64_t s1 = m_regs.load(op.opt[1]);
    uint64_t s2 = m_regs.load(op.opt[2]);
    m_regs.store(op.opt[0], s1 + s2);
}

void VM::print_int(Operation &op)
{
    if (m_debug)
        std::cerr << "PRINT_INT " << (int)op.opt[0] << "\n";
    std::cout << m_regs.load(op.opt[0]) << "\n";
}
#endif

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
#if 0
    Operation op = decode(fetch());
    switch (op.code) {
        case Opcode::NOP:
            std::cout << "NOP\n";
            break;
        case Opcode::LOAD:
            load(op);
            break;
        case Opcode::STORE:
            store(op);
            break;
        case Opcode::ADD:
            add(op);
            break;
        case Opcode::PRINT_INT:
            print_int(op);
            break;
        case Opcode::STOP:
            if (m_debug)
                std::cout << "STOP\n";
            return false;
        default:
            std::cout << "*** Invalid opcode: "<< (int)op.code <<"\n";
            //throw std::string("Invalid opcode");
            return false;
    }
#endif
    return true;
}

#include "regs.hh"

using core::Registers;

Registers::Registers() : m_pc(0)
{
    for (uint8_t i = 0; i < num_registers; ++i) {
        m_reg.push_back(RegisterData());
    }
}

void Registers::store_int(uint8_t num, uint64_t val)
{
    if (num == (uint8_t)-1) {
        m_pc = val;
        return;
    }
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg[num].m_type = core::RegisterType::Integer;
    m_reg[num].m_int = val;
}

void Registers::store_float(uint8_t num, double val)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg[num].m_type = core::RegisterType::Float;
    m_reg[num].m_float = val;
}

void Registers::store_string(uint8_t num, std::string val)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg[num].m_type = core::RegisterType::String;
    m_reg[num].m_str = val;
}

core::RegisterType Registers::type(uint8_t num)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    return m_reg[num].m_type;
}

uint64_t Registers::load_int(uint8_t num) const
{
    if (num == (uint8_t)-1) {
        return m_pc;
    }
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg[num].m_type != core::RegisterType::Integer)
        throw std::string("Invalid register type, expected integer");
    return m_reg[num].m_int;
}

double Registers::load_float(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg[num].m_type != core::RegisterType::Float)
        throw std::string("Invalid register type, expected float");
    return m_reg[num].m_float;
}

std::string Registers::load_string(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg[num].m_type != core::RegisterType::String)
        throw std::string("Invalid register type, expected string");
    return m_reg[num].m_str;
}

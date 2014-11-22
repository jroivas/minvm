#include "regs.hh"

using core::Registers;

Registers::Registers() : m_pc(0), m_reg_int{0}, m_reg_float{0}
{
    for (uint8_t i = 0; i < num_registers; ++i) {
        m_reg_type[i] = core::RegisterType::Integer;
    }
}

void Registers::store_int(uint8_t num, uint64_t val)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg_type[num] = core::RegisterType::Integer;
    m_reg_int[num] = val;
}

void Registers::store_float(uint8_t num, double val)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg_type[num] = core::RegisterType::Float;
    m_reg_float[num] = val;
}

void Registers::store_string(uint8_t num, std::string val)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg_type[num] = core::RegisterType::String;
    m_reg_string[num] = val;
}

core::RegisterType Registers::type(uint8_t num)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    return m_reg_type[num];
}

uint64_t Registers::load_int(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg_type[num] != core::RegisterType::Integer)
        throw std::string("Invalid register type, expected integer");
    return m_reg_int[num];
}

double Registers::load_float(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg_type[num] != core::RegisterType::Float)
        throw std::string("Invalid register type, expected float");
    return m_reg_float[num];
}

std::string Registers::load_string(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg_type[num] != core::RegisterType::String)
        throw std::string("Invalid register type, expected string");
    return m_reg_string[num];
}

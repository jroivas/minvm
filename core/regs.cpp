#include "regs.hh"
#include <sstream>
#include <iomanip>

using core::Registers;

Registers::Registers() : m_pc(0)
{
    for (uint8_t i = 0; i < num_registers; ++i) {
        m_reg.push_back(RegisterData());
    }
}

void Registers::put_int(uint8_t num, uint64_t val)
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

void Registers::put_float(uint8_t num, double val)
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    m_reg[num].m_type = core::RegisterType::Float;
    m_reg[num].m_float = val;
}

void Registers::put_string(uint8_t num, std::string val)
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

uint64_t Registers::get_int(uint8_t num) const
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

double Registers::get_float(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg[num].m_type != core::RegisterType::Float)
        throw std::string("Invalid register type, expected float");
    return m_reg[num].m_float;
}

std::string Registers::get_string(uint8_t num) const
{
    if (num >= num_registers)
        throw std::string("Invalid register");
    if (m_reg[num].m_type != core::RegisterType::String)
        throw std::string("Invalid register type, expected string");
    return m_reg[num].m_str;
}

void Registers::copy(uint8_t dest, uint8_t src)
{
    if (dest >= num_registers ||
        src >= num_registers)
        throw std::string("Invalid register");

    m_reg[dest] = m_reg[src];
}

std::string Registers::dump()
{
    std::stringstream ss;
    ss << "IP: ";
    ss << std::setw(20) << std::setfill(' ');
    ss << m_pc << "\n";
    ss << "Registers:\n";
    for (uint8_t i = 0; i < num_registers; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int)i;
        ss << ": ";
        ss << std::setw(20) << std::setfill(' ');
        if (m_reg[i].m_type == core::RegisterType::Integer)
            ss << m_reg[i].m_int;
        else if (m_reg[i].m_type == core::RegisterType::Float)
            ss << m_reg[i].m_float;
        else if (m_reg[i].m_type == core::RegisterType::String)
            ss << m_reg[i].m_str;
        ss << "\n";
    }
    return ss.str();
}

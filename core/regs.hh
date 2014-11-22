#pragma once

#include <cstdint>
#include <string>

namespace core
{

static const uint8_t num_registers = 16;

enum class RegisterType : uint8_t
{
    Integer,
    Float,
    String
};

class Registers
{
public:
    Registers();

    void store_int(uint8_t num, uint64_t val);
    void store_float(uint8_t num, double val);
    void store_string(uint8_t num, std::string val);

    RegisterType type(uint8_t num);

    uint64_t load_int(uint8_t num) const;
    double load_float(uint8_t num) const;
    std::string load_string(uint8_t num) const;

    inline uint64_t pc() const
    {
        return m_pc;
    }
    inline void next()
    {
        ++m_pc;
    }
    inline void reset_pc()
    {
        m_pc = 0;
    }

private:
    uint64_t m_pc;

    RegisterType m_reg_type[num_registers];
    uint64_t m_reg_int[num_registers];
    double m_reg_float[num_registers];
    std::string m_reg_string[num_registers];
};

}

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace core
{

static const uint8_t num_registers = 16;

enum class RegisterType : uint8_t
{
    Integer,
    Float,
    String
};

class RegisterData {
public:
    RegisterData() : m_type(RegisterType::Integer), m_int(0) {}

    RegisterType m_type;
    union {
        uint64_t m_int;
        double m_float;
    };
    std::string m_str;
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

    void copy(uint8_t dest, uint8_t src);

    inline uint64_t pc() const
    {
        return m_pc;
    }
    inline void pc_update(uint64_t val)
    {
        m_pc = val;
    }
    inline void next()
    {
        ++m_pc;
    }
    inline void pc_reset()
    {
        m_pc = 0;
    }
    std::string dump();

private:
    uint64_t m_pc;
    std::vector<RegisterData> m_reg;
};

}

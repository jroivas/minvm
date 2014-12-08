#pragma once

#include <cstdint>

namespace core
{

enum class Info : uint8_t
{
    Info = 0,

    Ticks,

    HeapSize,
    HeapStart,
};

class Opcode
{
public:
    Opcode() : m_value(0) {}
    Opcode(uint8_t val) : m_value(val) {}
    Opcode(const Opcode &val) : m_value(val.m_value) {}

    Opcode &operator=(const Opcode &other)
    {
        m_value = other.m_value;
    }

    bool operator==(const Opcode &other) const
    {
        return other.m_value == m_value;
    }
    bool operator!=(const Opcode &other) const
    {
        return other.m_value != m_value;
    }
    uint8_t operator*() const
    {
        return m_value;
    }
    uint8_t operator()() const
    {
        return m_value;
    }

    uint8_t value() const
    {
        return m_value;
    }

private:
    uint8_t m_value;
};

}

#pragma once
#include <cstring>
#include <iostream>

namespace core
{

class Heap
{
public:
    Heap(uint64_t pos, uint64_t size) :
        m_pos(pos), m_size(size)
    {
        m_data = new uint8_t[m_size]();
    }

    Heap(const Heap &other) :
        m_pos(other.m_pos), m_size(other.m_size)
    {
        m_data = new uint8_t[m_size]();
        std::memmove(m_data, other.m_data, m_size);
    }

    ~Heap()
    {
        delete[] m_data;
        m_data = nullptr;
    }


    inline uint64_t pos() const
    {
        return m_pos;
    }

    inline uint64_t size() const
    {
        return m_size;
    }

    inline bool valid(uint64_t index) const
    {
        return (m_data != nullptr)
            && (index >= m_pos)
            && (index < (m_pos + m_size));
    }

    uint8_t &operator[](uint64_t index) const
    {
        if (!valid(index))
            throw std::string("Heap memory access out of bounds");
        return m_data[index - m_pos];
    }

    uint8_t &operator[](uint64_t index)
    {
        if (!valid(index))
            throw std::string("Heap memory access out of bounds");
        return m_data[index - m_pos];
    }

private:
    uint64_t m_pos;
    uint64_t m_size;
    uint8_t *m_data;
};

}

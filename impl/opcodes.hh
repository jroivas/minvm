#pragma once
#include <core/opcodes.hh>

namespace impl
{

class Opcode : public core::Opcode
{
public:
    static core::Opcode NOP()            { return core::Opcode(0x00); }

    static core::Opcode STORE_INT()      { return core::Opcode(0x01); }
    static core::Opcode STORE_INT_MEM()  { return core::Opcode(0x02); }
    static core::Opcode LOAD_INT()       { return core::Opcode(0x03); }
    static core::Opcode LOAD_INT_MEM()   { return core::Opcode(0x04); }

    static core::Opcode LOAD_INT8()      { return core::Opcode(0x05); }
    static core::Opcode LOAD_INT16()     { return core::Opcode(0x06); }
    static core::Opcode LOAD_INT32()     { return core::Opcode(0x07); }
    static core::Opcode LOAD_INT64()     { return core::Opcode(0x08); }

    static core::Opcode LOAD_STR()       { return core::Opcode(0x09); }
    static core::Opcode LOAD_STR_MEM()   { return core::Opcode(0x0a); }
    static core::Opcode STORE_STR()      { return core::Opcode(0x0b); }
    static core::Opcode STORE_STR_MEM()  { return core::Opcode(0x0c); }

    static core::Opcode INC_INT()        { return core::Opcode(0x0d); }
    static core::Opcode DEC_INT()        { return core::Opcode(0x0e); }
    static core::Opcode ADD_INT()        { return core::Opcode(0x0f); }
    static core::Opcode SUB_INT()        { return core::Opcode(0x10); }
    static core::Opcode MUL_INT()        { return core::Opcode(0x11); }
    static core::Opcode DIV_INT()        { return core::Opcode(0x12); }
    static core::Opcode MOD_INT()        { return core::Opcode(0x13); }

    static core::Opcode PRINT_INT()      { return core::Opcode(0x14); }
    static core::Opcode PRINT_FLOAT()    { return core::Opcode(0x15); }
    static core::Opcode PRINT_STR()      { return core::Opcode(0x16); }

    static core::Opcode RANDOM()         { return core::Opcode(0x17); }

    static core::Opcode JMP8()           { return core::Opcode(0x18); }
    static core::Opcode JMP16()          { return core::Opcode(0x19); }
    static core::Opcode JMP32()          { return core::Opcode(0x1a); }
    static core::Opcode JMP64()          { return core::Opcode(0x1b); }
    static core::Opcode JMP_INT()        { return core::Opcode(0x1c); }

    static core::Opcode JMP_LE8()        { return core::Opcode(0x1d); }
    static core::Opcode JMP_LE16()       { return core::Opcode(0x1e); }
    static core::Opcode JMP_LE32()       { return core::Opcode(0x1f); }
    static core::Opcode JMP_LE64()       { return core::Opcode(0x20); }
    static core::Opcode JMP_LE_INT()     { return core::Opcode(0x21); }

    static core::Opcode MOV()            { return core::Opcode(0x22); }
    static core::Opcode HEAP()           { return core::Opcode(0x23); }
    static core::Opcode INFO()           { return core::Opcode(0x24); }

    static core::Opcode STOP()           { return core::Opcode(0xff); }
};

}

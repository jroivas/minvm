#include "ints.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;
using impl::Ints;

Ints::Ints(VM *vm)
{
    vm->opcode((uint32_t)Opcode::STORE_INT8, Ints::store_int8);
    vm->opcode((uint32_t)Opcode::STORE_INT16, Ints::store_int16);
    vm->opcode((uint32_t)Opcode::STORE_INT32, Ints::store_int32);
    vm->opcode((uint32_t)Opcode::STORE_INT64, Ints::store_int64);

    vm->opcode((uint32_t)Opcode::INC_INT, Ints::inc_int);
    vm->opcode((uint32_t)Opcode::DEC_INT, Ints::dec_int);

    vm->opcode((uint32_t)Opcode::ADD_INT, Ints::add_int);
    vm->opcode((uint32_t)Opcode::SUB_INT, Ints::sub_int);

    vm->opcode((uint32_t)Opcode::MUL_INT, Ints::mul_int);
    vm->opcode((uint32_t)Opcode::DIV_INT, Ints::div_int);
    vm->opcode((uint32_t)Opcode::MOD_INT, Ints::mod_int);

    vm->opcode((uint32_t)Opcode::PRINT_INT, Ints::print_int);
}

bool Ints::store_int8(VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_INT8\n";
    uint8_t reg = vm->fetch8();
    uint8_t val = vm->fetch8();
    vm->regs().store_int(reg, val);

    return true;
}

bool Ints::store_int16(VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_INT16\n";

    uint8_t reg = vm->fetch8();
    uint16_t val = 0;
    for (uint16_t i = 0; i < 2; ++i) {
        val <<= 8;
        val |= vm->fetch8();
    }
    vm->regs().store_int(reg, val);

    return true;
}
bool Ints::store_int32(VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_INT32\n";
    uint8_t reg = vm->fetch8();
    uint32_t val = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        val <<= 8;
        val |= vm->fetch8();
    }
    vm->regs().store_int(reg, val);

    return true;
}

bool Ints::store_int64(VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_INT64\n";
    uint8_t reg = vm->fetch8();
    uint64_t val = 0;
    for (uint32_t i = 0; i < 8; ++i) {
        val <<= 8;
        val |= vm->fetch8();
    }
    vm->regs().store_int(reg, val);

    return true;
}

bool Ints::inc_int(VM *vm)
{
    if (vm->debug()) std::cerr << "INC_INT\n";
    uint8_t reg1 = vm->fetch8();

    vm->regs().store_int(
        reg1,
        vm->regs().load_int(reg1) + 1);

    return true;
}

bool Ints::dec_int(VM *vm)
{
    if (vm->debug()) std::cerr << "DEC_INT\n";
    uint8_t reg1 = vm->fetch8();

    vm->regs().store_int(
        reg1,
        vm->regs().load_int(reg1) - 1);

    return true;
}

bool Ints::add_int(VM *vm)
{
    if (vm->debug()) std::cerr << "ADD_INT\n";
    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();
    uint8_t reg3 = vm->fetch8();

    vm->regs().store_int(
        reg1,
        vm->regs().load_int(reg2) +
        vm->regs().load_int(reg3));

    return true;
}

bool Ints::sub_int(VM *vm)
{
    if (vm->debug()) std::cerr << "SUB_INT\n";
    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();
    uint8_t reg3 = vm->fetch8();

    vm->regs().store_int(
        reg1,
        vm->regs().load_int(reg2) -
        vm->regs().load_int(reg3));

    return true;
}

bool Ints::mul_int(core::VM *vm)
{
    if (vm->debug()) std::cerr << "MUL_INT\n";
    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();
    uint8_t reg3 = vm->fetch8();

    vm->regs().store_int(
        reg1,
        vm->regs().load_int(reg2) *
        vm->regs().load_int(reg3));

    return true;
}

bool Ints::div_int(core::VM *vm)
{
    if (vm->debug()) std::cerr << "DIV_INT\n";
    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();
    uint8_t reg3 = vm->fetch8();

    uint64_t val1 = vm->regs().load_int(reg2);
    uint64_t val2 = vm->regs().load_int(reg3);

    if (val2 == 0)
        throw std::string("Divide by zero!");

    vm->regs().store_int(
        reg1, val1 / val2);

    return true;
}

bool Ints::mod_int(core::VM *vm)
{
    if (vm->debug()) std::cerr << "MOD_INT\n";
    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();
    uint8_t reg3 = vm->fetch8();

    uint64_t val1 = vm->regs().load_int(reg2);
    uint64_t val2 = vm->regs().load_int(reg3);

    if (val2 == 0)
        throw std::string("Divide by zero!");

    vm->regs().store_int(
        reg1, val1 % val2);

    return true;
}

bool Ints::print_int(VM *vm)
{
    if (vm->debug()) std::cerr << "PRINT_INT\n";
    uint8_t reg = vm->fetch8();
    std::cout << vm->regs().load_int(reg);
    return true;
}

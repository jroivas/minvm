#include "asm1.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;

ASM1::ASM1(VM *vm)
{
    vm->opcode((uint32_t)Opcode::STORE_INT8, ASM1::store_int8);
    vm->opcode((uint32_t)Opcode::STORE_INT32, ASM1::store_int32);
    vm->opcode((uint32_t)Opcode::STORE_INT64, ASM1::store_int64);
    vm->opcode((uint32_t)Opcode::ADD_INT, ASM1::add_int);
    vm->opcode((uint32_t)Opcode::PRINT_INT, ASM1::print_int);

    vm->opcode((uint32_t)Opcode::STORE_STR, ASM1::store_str);
    vm->opcode((uint32_t)Opcode::PRINT_STR, ASM1::print_str);
}

bool ASM1::store_int8(VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_INT8\n";
    uint8_t reg = vm->fetch8();
    uint8_t val = vm->fetch8();
    vm->regs().store_int(reg, val);

    return true;
}

bool ASM1::store_int32(VM *vm)
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

bool ASM1::store_int64(VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_INT64\n";
    uint8_t reg = vm->fetch8();
    uint64_t val = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        val <<= 8;
        val |= vm->fetch8();
    }
    vm->regs().store_int(reg, val);

    return true;
}

bool ASM1::add_int(VM *vm)
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

bool ASM1::print_int(VM *vm)
{
    if (vm->debug()) std::cerr << "PRINT_INT\n";
    uint8_t reg = vm->fetch8();
    std::cout << vm->regs().load_int(reg);
    return true;
}

bool ASM1::store_str(core::VM *vm)
{
    if (vm->debug()) std::cerr << "STORE_STR\n";
    std::string res = "";
    uint8_t reg = vm->fetch8();

    uint8_t val = vm->fetch8();
    while (val != 0) {
        res.push_back(val);
        val = vm->fetch8();
    }
    vm->regs().store_string(reg, res);

    return true;
}

bool ASM1::print_str(core::VM *vm)
{
    if (vm->debug()) std::cerr << "PRINT_STR\n";
    uint8_t reg = vm->fetch8();

    std::cout << vm->regs().load_string(reg);

    return true;
}

#include "jump.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;
using impl::Jump;

Jump::Jump(VM *vm)
{
    vm->opcode((uint32_t)Opcode::JMP8, Jump::jump8);
    vm->opcode((uint32_t)Opcode::JMP16, Jump::jump16);
    vm->opcode((uint32_t)Opcode::JMP32, Jump::jump32);
    vm->opcode((uint64_t)Opcode::JMP64, Jump::jump64);
    vm->opcode((uint32_t)Opcode::JMP_INT, Jump::jump_int);

    vm->opcode((uint32_t)Opcode::JMP_LE8, Jump::jump_le8);
    vm->opcode((uint32_t)Opcode::JMP_LE16, Jump::jump_le16);
    vm->opcode((uint32_t)Opcode::JMP_LE32, Jump::jump_le32);
    vm->opcode((uint64_t)Opcode::JMP_LE64, Jump::jump_le64);
    vm->opcode((uint32_t)Opcode::JMP_LE_INT, Jump::jump_le_int);
}

bool Jump::conditional(
    core::VM *vm, uint8_t algo, uint8_t reg1, uint8_t reg2)
{
    uint64_t val1 = vm->regs().load_int(reg1);
    uint64_t val2 = vm->regs().load_int(reg2);

    switch (algo) {
        case 0: return val1 == val2;
        case 1: return val1 < val2;
        case 2: return val1 > val2;
        case 3: return val1 <= val2;
        case 4: return val1 >= val2;
        case 5: return val1 != val2;
        case 6: return !val1;
        case 7: return !val2;
        case 8: return val1 && val2;
        case 9: return val1 || val2;
        case 10: return val1 || ~val2;
        case 11: return ~val1 || val2;
        case 12: return val1 && ~val2;
        case 13: return ~val1 && val2;
        case 14: return val1 ^ val2;
        case 15: return val1 % val2;
        default:
            throw std::string("Invalid comparison in jump");
    }
}

void Jump::jump_conditional(
    core::VM *vm, uint64_t addr, bool cond)
{
    if (!cond) return;

    vm->regs().pc_update(addr);
}

bool Jump::jump8(core::VM *vm)
{
    if (vm->debug()) std::cerr << "JUMP8\n";

    jump_conditional(
        vm,
        vm->regs().pc() + vm->fetch8(),
        true);

    return true;
}

bool Jump::jump16(core::VM *vm)
{
    if (vm->debug()) std::cerr << "JUMP16\n";

    uint16_t raw = vm->fetch8();
    raw <<= 8;
    raw |= vm->fetch8();
    int16_t diff = static_cast<int16_t>(raw);

    jump_conditional(
        vm,
        vm->regs().pc() + diff,
        true);

    return true;
}

bool Jump::jump32(core::VM *vm)
{
    if (vm->debug()) std::cerr << "JUMP16\n";

    uint32_t raw = vm->fetch8();
    raw <<= 8;
    raw |= vm->fetch8();
    raw <<= 8;
    raw |= vm->fetch8();
    raw <<= 8;
    raw |= vm->fetch8();
    int32_t diff = static_cast<int32_t>(raw);

    jump_conditional(
        vm,
        vm->regs().pc() + diff,
        true);

    return true;
}

bool Jump::jump64(core::VM *vm)
{
    uint64_t diff = 0;
    for (int i = 0; i < 8; ++i) {
        diff <<= 8;
        diff |= vm->fetch8();
    }

    jump_conditional(
        vm,
        diff,
        true);

    return true;
}

bool Jump::jump_int(core::VM *vm)
{
    if (vm->debug()) std::cerr << "JUMP_INT\n";

    int8_t reg = vm->fetch8();
    uint64_t diff = vm->regs().load_int(reg);

    vm->regs().pc_update(
        vm->regs().pc() + diff);

    return true;
}

bool Jump::jump_le8(core::VM *vm)
{
    int8_t oper = vm->fetch8();
    int8_t reg1 = vm->fetch8();
    int8_t reg2 = vm->fetch8();

    bool cond = conditional(vm, oper, reg1, reg2);

    jump_conditional(
        vm,
        vm->regs().pc() + vm->fetch8(),
        cond);

    return true;
}

bool Jump::jump_le16(core::VM *vm)
{
    int8_t oper = vm->fetch8();
    int8_t reg1 = vm->fetch8();
    int8_t reg2 = vm->fetch8();

    bool cond = conditional(vm, oper, reg1, reg2);

    uint16_t raw = vm->fetch8();
    raw <<= 8;
    raw |= vm->fetch8();
    int16_t diff = static_cast<int16_t>(raw);

    jump_conditional(
        vm,
        vm->regs().pc() + diff,
        cond);

    return true;
}

bool Jump::jump_le32(core::VM *vm)
{
    int8_t oper = vm->fetch8();
    int8_t reg1 = vm->fetch8();
    int8_t reg2 = vm->fetch8();

    bool cond = conditional(vm, oper, reg1, reg2);

    uint32_t raw = 0;
    for (int i = 0; i < 4; ++i) {
        raw <<= 8;
        raw |= vm->fetch8();
    }
    int32_t diff = static_cast<int32_t>(raw);

    jump_conditional(
        vm,
        vm->regs().pc() + diff,
        cond);


    return true;
}

bool Jump::jump_le64(core::VM *vm)
{
    int8_t oper = vm->fetch8();
    int8_t reg1 = vm->fetch8();
    int8_t reg2 = vm->fetch8();

    bool cond = conditional(vm, oper, reg1, reg2);

    uint64_t diff = 0;
    for (int i = 0; i < 8; ++i) {
        diff <<= 8;
        diff |= vm->fetch8();
    }

    jump_conditional(
        vm,
        diff,
        cond);

    return true;
}

bool Jump::jump_le_int(core::VM *vm)
{
    int8_t oper = vm->fetch8();
    int8_t reg1 = vm->fetch8();
    int8_t reg2 = vm->fetch8();

    bool cond = conditional(vm, oper, reg1, reg2);

    int8_t reg = vm->fetch8();
    uint64_t diff = vm->regs().load_int(reg);

    jump_conditional(
        vm,
        diff,
        cond);

    return true;
}

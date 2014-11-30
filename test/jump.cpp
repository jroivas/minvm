#include "framework.hh"
#include <vm.hh>
#include <opcodes.hh>
#include <nopstop.hh>
#include <ints.hh>
#include <jump.hh>

static void test_jump_jmp8()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 1,
        (uint8_t)core::Opcode::JMP8, 3,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    uint64_t pos = vm.regs().pc();
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.regs().pc() == pos + 4);
    assert(vm.regs().get_int(0) == 1);

    assert(!vm.step());
}

static void test_jump_jmp8_neg()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 1,
        (uint8_t)core::Opcode::JMP8, uint8_t(-4),
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    uint64_t pos = vm.regs().pc();
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.regs().pc() == 0);
    assert(vm.regs().get_int(0) == 1);

    assert(vm.step());
    assert(vm.regs().pc() == 3);
    assert(vm.regs().get_int(0) == 1);
}

static void test_jump_jmp16()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 1,
        (uint8_t)core::Opcode::JMP16, 0, 4,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    uint64_t pos = vm.regs().pc();
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.regs().pc() == pos + 5);
    assert(vm.regs().get_int(0) == 1);

    assert(!vm.step());
}

static void test_jump_jmp32()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 1,
        (uint8_t)core::Opcode::JMP32, 0, 0, 0, 6,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    uint64_t pos = vm.regs().pc();
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.regs().pc() == pos + 7);
    assert(vm.regs().get_int(0) == 1);

    assert(!vm.step());
}

static void test_jump_jmp64()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 1,
        (uint8_t)core::Opcode::JMP64, 0, 0, 0, 0, 0, 0, 0, 14,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    uint64_t pos = vm.regs().pc();
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.regs().pc() == 14);
    assert(vm.regs().get_int(0) == 1);

    assert(!vm.step());
}

static void test_jump_jmp_int()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 1,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 10,
        (uint8_t)core::Opcode::JMP_INT, 1,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);
    assert(vm.regs().get_int(1) == 10);

    uint64_t pos = vm.regs().pc();
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.regs().pc() == 10);
    assert(vm.regs().get_int(0) == 1);

    assert(!vm.step());
}

static void test_jump_jmp_le8()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 2,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::JMP_LE8, 1, 0, 1, (uint8_t)-6,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);
    assert(vm.regs().get_int(1) == 2);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    uint64_t pos = vm.regs().pc();
    assert(vm.step());

    assert(vm.regs().pc() == 6);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    assert(vm.step());
    assert(vm.regs().pc() == pos);
    assert(vm.regs().get_int(0) == vm.regs().get_int(1));

    assert(vm.step());
    assert(!vm.step());
}

static void test_jump_jmp_le16()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 2,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::JMP_LE16, 1, 0, 1, 0, 0,
        (uint8_t)core::Opcode::STOP
    };
    uint16_t neg = (uint16_t)-6;
    mem[12] = neg >> 8;
    mem[13] = neg & 0xFF;

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);
    assert(vm.regs().get_int(1) == 2);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    uint64_t pos = vm.regs().pc();
    assert(vm.step());

    assert(vm.regs().pc() == 6);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    assert(vm.step());
    assert(vm.regs().pc() == pos);
    assert(vm.regs().get_int(0) == vm.regs().get_int(1));

    assert(vm.step());
    assert(!vm.step());
}

static void test_jump_jmp_le32()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 2,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::JMP_LE32, 1, 0, 1, 0, 0, 0, 0,
        (uint8_t)core::Opcode::STOP
    };
    uint32_t neg = (uint32_t)-6;
    mem[12] = (neg >> 24) & 0xFF;
    mem[13] = (neg >> 16) & 0xFF;
    mem[14] = (neg >> 8) & 0xFF;
    mem[15] = neg & 0xFF;

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);
    assert(vm.regs().get_int(1) == 2);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    uint64_t pos = vm.regs().pc();
    assert(vm.step());

    assert(vm.regs().pc() == 6);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    assert(vm.step());
    assert(vm.regs().pc() == pos);
    assert(vm.regs().get_int(0) == vm.regs().get_int(1));

    assert(vm.step());
    assert(!vm.step());
}

static void test_jump_jmp_le64()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 2,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::JMP_LE64, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 6,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);
    assert(vm.regs().get_int(1) == 2);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    uint64_t pos = vm.regs().pc();
    assert(vm.step());

    assert(vm.regs().pc() == 6);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    assert(vm.step());
    assert(vm.regs().pc() == pos);
    assert(vm.regs().get_int(0) == vm.regs().get_int(1));

    assert(vm.step());
    assert(!vm.step());
}

static void test_jump_jmp_le_int()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 2,
        (uint8_t)core::Opcode::LOAD_INT8, 2, 9,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::JMP_LE_INT, 1, 0, 1, 2,
        (uint8_t)core::Opcode::STOP
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Jump jmps(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);
    assert(vm.regs().get_int(1) == 2);
    assert(vm.regs().get_int(2) == 9);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    uint64_t pos = vm.regs().pc();
    assert(vm.step());

    assert(vm.regs().pc() == 9);
    assert(vm.regs().get_int(0) != vm.regs().get_int(1));

    assert(vm.step());
    assert(vm.regs().pc() == pos);
    assert(vm.regs().get_int(0) == vm.regs().get_int(1));

    assert(vm.step());
    assert(!vm.step());
}


void test_jump()
{
    TEST_CASE(test_jump_jmp8);
    TEST_CASE(test_jump_jmp8_neg);
    TEST_CASE(test_jump_jmp16);
    TEST_CASE(test_jump_jmp32);
    TEST_CASE(test_jump_jmp64);
    TEST_CASE(test_jump_jmp_int);

    TEST_CASE(test_jump_jmp_le8);
    TEST_CASE(test_jump_jmp_le16);
    TEST_CASE(test_jump_jmp_le32);
    TEST_CASE(test_jump_jmp_le64);
    TEST_CASE(test_jump_jmp_le_int);
}

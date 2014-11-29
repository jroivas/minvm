#include "framework.hh"
#include <vm.hh>
#include <opcodes.hh>
#include <ints.hh>

static void test_ints_load_int8()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x42
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0x42);
}

static void test_ints_load_int16()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT16, 1, 0x42, 0x33
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4233);
}

static void test_ints_load_int32()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT32, 1, 0x42, 0x33, 0x21, 0x15
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x42332115);
}

static void test_ints_load_int64()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT64,
            1, 0x42, 0x33, 0x21, 0x15, 1, 2, 3, 4
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4233211501020304);
}

static void test_ints_inc()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT16, 1, 0x42, 0x33,
        (uint8_t)core::Opcode::INC_INT, 0,
        (uint8_t)core::Opcode::INC_INT, 1,
        (uint8_t)core::Opcode::INC_INT, 1
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4233);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 1);

    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4234);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4235);
}

static void test_ints_dec()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT16, 1, 0x42, 0x33,
        (uint8_t)core::Opcode::DEC_INT, 0,
        (uint8_t)core::Opcode::DEC_INT, 1,
        (uint8_t)core::Opcode::DEC_INT, 1
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4233);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == static_cast<uint64_t>(-1));

    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4232);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4231);
}

static void test_ints_add()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x0a,
        (uint8_t)core::Opcode::ADD_INT, 0, 1, 1,
        (uint8_t)core::Opcode::ADD_INT, 2, 0, 0,
        (uint8_t)core::Opcode::ADD_INT, 3, 1, 0,
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x0a);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0x0a * 2);

    assert(vm.step());
    assert(vm.regs().get_int(2) == 0x0a * 2 * 2);
    assert(vm.step());
    assert(vm.regs().get_int(3) == 0x0a * 3);
}

static void test_ints_sub()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0x02,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x0a,
        (uint8_t)core::Opcode::SUB_INT, 3, 1, 0,
        (uint8_t)core::Opcode::SUB_INT, 4, 3, 0,
        (uint8_t)core::Opcode::SUB_INT, 5, 4, 1,
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0x02);
    assert(vm.regs().get_int(1) == 0x0a);

    assert(vm.step());
    assert(vm.regs().get_int(3) == 0x0a - 0x02);

    assert(vm.step());
    assert(vm.regs().get_int(4) == 0x0a - 0x02 * 2);

    assert(vm.step());
    assert(vm.regs().get_int(5) == static_cast<uint64_t>(-0x02 * 2));
}

static void test_ints_mul()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0x02,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x0a,
        (uint8_t)core::Opcode::MUL_INT, 3, 1, 0,
        (uint8_t)core::Opcode::MUL_INT, 4, 3, 0,
        (uint8_t)core::Opcode::MUL_INT, 5, 4, 1,
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0x02);
    assert(vm.regs().get_int(1) == 0x0a);

    assert(vm.step());
    assert(vm.regs().get_int(3) == 0x0a * 2);

    assert(vm.step());
    assert(vm.regs().get_int(4) == 0x0a * 2 * 2);

    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x0a * 4 * 0x0a);
}

static void test_ints_div()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0x02,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x0a,
        (uint8_t)core::Opcode::DIV_INT, 3, 1, 0,
        (uint8_t)core::Opcode::DIV_INT, 4, 3, 0,
        (uint8_t)core::Opcode::DIV_INT, 5, 4, 1,
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0x02);
    assert(vm.regs().get_int(1) == 0x0a);

    assert(vm.step());
    assert(vm.regs().get_int(3) == 5);

    assert(vm.step());
    assert(vm.regs().get_int(4) == 2);

    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x0a / 4 / 0x0a);
}

static void test_ints_mod()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0x02,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x0a,
        (uint8_t)core::Opcode::LOAD_INT8, 2, 3,
        (uint8_t)core::Opcode::MOD_INT, 3, 1, 0,
        (uint8_t)core::Opcode::MOD_INT, 4, 1, 2,
        (uint8_t)core::Opcode::MOD_INT, 5, 0, 2,
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0x02);
    assert(vm.regs().get_int(1) == 0x0a);
    assert(vm.regs().get_int(2) == 0x03);

    assert(vm.step());
    assert(vm.regs().get_int(3) == 0);

    assert(vm.step());
    assert(vm.regs().get_int(4) == 1);

    assert(vm.step());
    assert(vm.regs().get_int(5) == 2);
}

static void test_ints_print()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 0x02,
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x42,
        (uint8_t)core::Opcode::PRINT_INT, 0,
        (uint8_t)core::Opcode::PRINT_INT, 1,
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0x02);
    assert(vm.regs().get_int(1) == 0x42);

    StdoutCatcher catcher;
    catcher.start();
    bool res = vm.step();
    catcher.stop();
    assert(res);

    assert(catcher.get() == "2");

    catcher.start(true);
    res = vm.step();
    catcher.stop();
    assert(res);

    assert(catcher.get() == "66");
}

static void test_ints_div_by_zero()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x02,
        (uint8_t)core::Opcode::DIV_INT, 3, 1, 0
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 2);

    assertThrows(
        std::string,
        "Divide by zero!",
        vm.step());
}

static void test_ints_mod_by_zero()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 1, 0x02,
        (uint8_t)core::Opcode::MOD_INT, 3, 1, 0
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0);
    assert(vm.regs().get_int(1) == 2);

    assertThrows(
        std::string,
        "Divide by zero!",
        vm.step());
}

void test_ints()
{
    TEST_CASE(test_ints_load_int8);
    TEST_CASE(test_ints_load_int16);
    TEST_CASE(test_ints_load_int32);
    TEST_CASE(test_ints_load_int64);

    TEST_CASE(test_ints_inc);
    TEST_CASE(test_ints_dec);

    TEST_CASE(test_ints_add);
    TEST_CASE(test_ints_sub);

    TEST_CASE(test_ints_mul);
    TEST_CASE(test_ints_div);
    TEST_CASE(test_ints_div_by_zero);
    TEST_CASE(test_ints_mod);
    TEST_CASE(test_ints_mod_by_zero);

    TEST_CASE(test_ints_print);
}

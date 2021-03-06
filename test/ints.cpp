#include "framework.hh"
#include <vm.hh>
#include <impl/opcodes.hh>
#include <ints.hh>
#include <iomanip>

static void test_ints_load_int8()
{
    static uint8_t mem[] = {
        *impl::Opcode::LOAD_INT8(), 1, 0x42
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
        *impl::Opcode::LOAD_INT16(), 1, 0x42, 0x33
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
        *impl::Opcode::LOAD_INT32(), 1, 0x42, 0x33, 0x21, 0x15
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
        *impl::Opcode::LOAD_INT64(),
            1, 0x42, 0x33, 0x21, 0x15, 1, 2, 3, 4
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(1) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(1) == 0x4233211501020304);
}

static uint8_t mem2[] = {
    *impl::Opcode::LOAD_INT_MEM(),
        5, 1, 0, 0, 0, 0, 0, 0, 0, 12,
    *impl::Opcode::STOP(),
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x50
};

static void test_ints_load_int_mem_1byte()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 1;
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x42);
}

static void test_ints_load_int_mem_2bytes()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 2;
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x4243);
}

static void test_ints_load_int_mem_2bytes_overflow()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 2;
    mem2[2] = 22;
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(5) == 0);
    assertThrows(
        std::string,
        "Invalid size: 22",
        vm.step());
}

static void test_ints_load_int_mem_3bytes()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 3;
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x424344);
}

static void test_ints_load_int_mem_4bytes()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 4;
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x42434445);
}

static void test_ints_load_int_mem_8bytes()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 8;
    impl::Ints ints(&vm);

    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x4243444546474849);
}

static void test_ints_load_int_mem_9bytes()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    mem2[2] = 9;

    impl::Ints ints(&vm);
    assert(vm.regs().get_int(5) == 0);

    assertThrows(
        std::string,
        "Invalid size: 9",
        vm.step());
}

static uint8_t mem3[] = {
    *impl::Opcode::LOAD_INT8(), 2, 8,
    *impl::Opcode::LOAD_INT(), 5, 1, 2,
    *impl::Opcode::STOP(),
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x50
};

static void test_ints_load_int_1byte()
{
    core::VM vm((uint8_t*)mem3, sizeof(mem3));
    mem3[5] = 1;
    impl::Ints ints(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x42);
}

static void test_ints_load_int_3bytes()
{
    core::VM vm((uint8_t*)mem3, sizeof(mem3));
    mem3[5] = 3;
    impl::Ints ints(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x424344);
}

static void test_ints_load_int_4bytes()
{
    core::VM vm((uint8_t*)mem3, sizeof(mem3));
    mem3[5] = 4;
    impl::Ints ints(&vm);

    assert(vm.step());
    assert(vm.regs().get_int(5) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(5) == 0x42434445);
}

static void test_ints_inc()
{
    static uint8_t mem[] = {
        *impl::Opcode::LOAD_INT16(), 1, 0x42, 0x33,
        *impl::Opcode::INC_INT(), 0,
        *impl::Opcode::INC_INT(), 1,
        *impl::Opcode::INC_INT(), 1
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
        *impl::Opcode::LOAD_INT16(), 1, 0x42, 0x33,
        *impl::Opcode::DEC_INT(), 0,
        *impl::Opcode::DEC_INT(), 1,
        *impl::Opcode::DEC_INT(), 1
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
        *impl::Opcode::LOAD_INT8(), 1, 0x0a,
        *impl::Opcode::ADD_INT(), 0, 1, 1,
        *impl::Opcode::ADD_INT(), 2, 0, 0,
        *impl::Opcode::ADD_INT(), 3, 1, 0,
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
        *impl::Opcode::LOAD_INT8(), 0, 0x02,
        *impl::Opcode::LOAD_INT8(), 1, 0x0a,
        *impl::Opcode::SUB_INT(), 3, 1, 0,
        *impl::Opcode::SUB_INT(), 4, 3, 0,
        *impl::Opcode::SUB_INT(), 5, 4, 1,
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
        *impl::Opcode::LOAD_INT8(), 0, 0x02,
        *impl::Opcode::LOAD_INT8(), 1, 0x0a,
        *impl::Opcode::MUL_INT(), 3, 1, 0,
        *impl::Opcode::MUL_INT(), 4, 3, 0,
        *impl::Opcode::MUL_INT(), 5, 4, 1,
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
        *impl::Opcode::LOAD_INT8(), 0, 0x02,
        *impl::Opcode::LOAD_INT8(), 1, 0x0a,
        *impl::Opcode::DIV_INT(), 3, 1, 0,
        *impl::Opcode::DIV_INT(), 4, 3, 0,
        *impl::Opcode::DIV_INT(), 5, 4, 1,
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
        *impl::Opcode::LOAD_INT8(), 0, 0x02,
        *impl::Opcode::LOAD_INT8(), 1, 0x0a,
        *impl::Opcode::LOAD_INT8(), 2, 3,
        *impl::Opcode::MOD_INT(), 3, 1, 0,
        *impl::Opcode::MOD_INT(), 4, 1, 2,
        *impl::Opcode::MOD_INT(), 5, 0, 2,
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
        *impl::Opcode::LOAD_INT8(), 0, 0x02,
        *impl::Opcode::LOAD_INT8(), 1, 0x42,
        *impl::Opcode::PRINT_INT(), 0,
        *impl::Opcode::PRINT_INT(), 1,
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
        *impl::Opcode::LOAD_INT8(), 1, 0x02,
        *impl::Opcode::DIV_INT(), 3, 1, 0
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
        *impl::Opcode::LOAD_INT8(), 1, 0x02,
        *impl::Opcode::MOD_INT(), 3, 1, 0
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

    TEST_CASE(test_ints_load_int_1byte);
    TEST_CASE(test_ints_load_int_3bytes);
    TEST_CASE(test_ints_load_int_4bytes);

    TEST_CASE(test_ints_load_int_mem_1byte);
    TEST_CASE(test_ints_load_int_mem_2bytes);
    TEST_CASE(test_ints_load_int_mem_2bytes_overflow);
    TEST_CASE(test_ints_load_int_mem_3bytes);
    TEST_CASE(test_ints_load_int_mem_4bytes);
    TEST_CASE(test_ints_load_int_mem_8bytes);
    TEST_CASE(test_ints_load_int_mem_9bytes);

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

#include "framework.hh"
#include <vm.hh>
#include <opcodes.hh>
#include <strs.hh>

static uint8_t mem[] = {
    (uint8_t)core::Opcode::LOAD_STR, 0, 'A', 'b', 'E', 0x9, 0,
    (uint8_t)core::Opcode::LOAD_STR, 1, 'R', 0,
    (uint8_t)core::Opcode::LOAD_STR, 2, 'D', 'u', 'm', 'm', 'y', 0,
    (uint8_t)core::Opcode::PRINT_STR, 0,
    (uint8_t)core::Opcode::PRINT_STR, 2,
};

static void test_strs_load_str()
{
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Strs strs(&vm);

    assert(vm.step());
    assert(vm.regs().get_string(0) == "AbE\x09");

    assert(vm.step());
    assert(vm.regs().get_string(1) == "R");

    assert(vm.step());
    assert(vm.regs().get_string(2) == "Dummy");
}

static void test_strs_print()
{

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Strs strs(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());

    StdoutCatcher catcher;
    catcher.start();
    bool res = vm.step();
    catcher.stop();
    assert(res);

    assert(catcher.get() == "AbE\x09");

    catcher.start(true);
    res = vm.step();
    catcher.stop();
    assert(res);

    assert(catcher.get() == "Dummy");
}

void test_strs()
{
    TEST_CASE(test_strs_load_str);

    TEST_CASE(test_strs_print);
}

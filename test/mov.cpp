#include "framework.hh"
#include <ints.hh>
#include <strs.hh>
#include <mov.hh>

static uint8_t mem[] = {
    (uint8_t)core::Opcode::LOAD_INT8, 0, 42,
    (uint8_t)core::Opcode::LOAD_STR, 5, 'a', 'b', 0,
    (uint8_t)core::Opcode::MOV, 1, 0,
    (uint8_t)core::Opcode::MOV, 10, 5,
};

static void test_basic_mov()
{
    core::VM vm((uint8_t*)mem, sizeof(mem));

    impl::Ints ints(&vm);
    impl::Strs strs(&vm);
    impl::Mov mov(&vm);

    assert(vm.step());
    assert(vm.step());

    assert(vm.regs().get_int(0) == 42);
    assert(vm.regs().get_int(1) == 0);
    assert(vm.regs().get_string(5) == "ab");
    assert(vm.regs().get_int(10) == 0);

    assert(vm.step());
    assert(vm.regs().get_int(0) == 42);
    assert(vm.regs().get_int(1) == 42);

    assert(vm.step());
    assert(vm.regs().get_string(5) == "ab");
    assert(vm.regs().get_string(10) == "ab");
}

void test_mov()
{
    TEST_CASE(test_basic_mov);
}

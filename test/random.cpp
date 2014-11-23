#include "framework.hh"
#include <vm.hh>
#include <opcodes.hh>
#include <random.hh>

static uint8_t mem[] = {
    (uint8_t)core::Opcode::RANDOM, 0,
    (uint8_t)core::Opcode::RANDOM, 1
};

static void test_random_ints()
{
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Random random(&vm);

    // In fact it's possible for these tests to fail,
    // but possibility for that is minimal...

    assert(vm.regs().load_int(0) == 0);
    assert(vm.regs().load_int(1) == 0);

    assert(vm.step());
    assert(vm.regs().load_int(0) != 0);
    assert(vm.regs().load_int(1) == 0);

    assert(vm.step());
    assert(vm.regs().load_int(0) != 0);
    assert(vm.regs().load_int(1) != 0);

    assert(vm.regs().load_int(0) != vm.regs().load_int(1));
}

void test_random()
{
    TEST_CASE(test_random_ints);
}

#include "framework.hh"
#include <vm.hh>
#include <opcodes.hh>
#include <ints.hh>
#include <strs.hh>

static uint8_t mem1[] = {
    (uint8_t)core::Opcode::STORE_INT8, 0, 5,
    (uint8_t)core::Opcode::STORE_INT8, 1, 10,
    (uint8_t)core::Opcode::STORE_STR, 8, 't', 's', 't', 0
};

static uint8_t mem2[] = {
    (uint8_t)core::Opcode::STORE_INT8, 0, 123,
    (uint8_t)core::Opcode::STORE_INT8, 1, 99,
    (uint8_t)core::Opcode::STORE_STR, 8, 'a', 'b', 'c', 0,
    (uint8_t)core::Opcode::STOP
};

static uint8_t mem3[] = {
    (uint8_t)core::Opcode::STORE_INT8, 0, 5
};

static void test_basic_opcodes()
{
    core::VM vm;

    // Get default NOP
    assert(vm.get_opcode(0)(&vm));
    assert(vm.get_opcode(1)(&vm));
    assert(vm.get_opcode(100)(&vm));

    // Test default stop
    assert(!vm.get_opcode((uint8_t)core::Opcode::STOP)(&vm));
}

static void test_memory_fetch()
{
    core::VM vm;

    vm.load((uint8_t*)mem1, sizeof(mem1));

    assert(vm.fetch() == core::Opcode::STORE_INT8);
    assert(vm.fetch8() == 0);
    assert(vm.fetch8() == 5);

    assert(vm.fetch() == core::Opcode::STORE_INT8);
    assert(vm.fetch8() == 1);
    assert(vm.fetch8() == 10);

    assert(vm.fetch() == core::Opcode::STORE_STR);
    assert(vm.fetch8() == 8);
    assert(vm.fetch8() == 't');
    assert(vm.fetch8() == 's');
    assert(vm.fetch8() == 't');
    assert(vm.fetch8() == 0);
}

static void test_memory_step()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));

    // There's no int and str opcodes registered
    // so it will execute NOP on every value

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().pc() == 3);
};

static void test_memory_exe()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    impl::Ints ints(&vm);
    impl::Strs strs(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());

    assert(vm.regs().pc() == 12);
    assert(vm.regs().load_string(8) == "abc");

    assert(!vm.step());
    assert(vm.regs().pc() == 13);
    assert(vm.regs().load_string(8) == "abc");
}

static void test_memory_limits()
{
    core::VM vm((uint8_t*)mem3, sizeof(mem3));

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());

    assertThrows(
        std::string,
        "Memory access out of bounds",
        vm.step());
    assertThrows(
        std::string,
        "Memory access out of bounds",
        vm.fetch());
    assertThrows(
        std::string,
        "Memory access out of bounds",
        vm.fetch8());
}

void test_vm()
{
    TEST_CASE(test_basic_opcodes);
    TEST_CASE(test_memory_fetch);
    TEST_CASE(test_memory_step);
    TEST_CASE(test_memory_exe);
    TEST_CASE(test_memory_limits);
}

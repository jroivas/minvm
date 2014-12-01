#include "framework.hh"
#include <vm.hh>
#include <opcodes.hh>
#include <nopstop.hh>
#include <ints.hh>
#include <strs.hh>

static uint8_t mem1[] = {
    (uint8_t)core::Opcode::LOAD_INT8, 0, 5,
    (uint8_t)core::Opcode::LOAD_INT8, 1, 10,
    (uint8_t)core::Opcode::LOAD_STR, 8, 't', 's', 't', 0
};

static uint8_t mem2[] = {
    (uint8_t)core::Opcode::LOAD_INT8, 0, 123,
    (uint8_t)core::Opcode::LOAD_INT8, 1, 99,
    (uint8_t)core::Opcode::LOAD_STR, 8, 'a', 'b', 'c', 0,
    (uint8_t)core::Opcode::STOP
};

static uint8_t mem3[] = {
    (uint8_t)core::Opcode::LOAD_INT8, 0, 5
};

static void test_basic_opcodes()
{
    core::VM vm;

    // No default NOP
    assertThrows(
        std::string,
        "Invalid opcode",
        vm.get_opcode((uint8_t)core::Opcode::NOP)(&vm));
    // No default STOP
    assertThrows(
        std::string,
        "Invalid opcode",
        vm.get_opcode((uint8_t)core::Opcode::STOP)(&vm));

    // No other opcodes
    assertThrows(
        std::string,
        "Invalid opcode",
        vm.get_opcode(100)(&vm));

    impl::NopStop nopstop(&vm);

    // Test NOP
    assert(vm.get_opcode((uint8_t)core::Opcode::NOP)(&vm));

    // Test STOP
    assert(!vm.get_opcode((uint8_t)core::Opcode::STOP)(&vm));
}

static void test_empty_vm()
{
    core::VM vm;

    assertThrows(
        std::string,
        "Memory access out of bounds",
        vm.step());

    vm.load(nullptr, 10);
    assertThrows(
        std::string,
        "Invalid memory",
        vm.step());
}

static void test_memory_fetch()
{
    core::VM vm;

    vm.load((uint8_t*)mem1, sizeof(mem1));

    assert(vm.fetch() == core::Opcode::LOAD_INT8);
    assert(vm.fetch8() == 0);
    assert(vm.fetch8() == 5);

    assert(vm.fetch() == core::Opcode::LOAD_INT8);
    assert(vm.fetch8() == 1);
    assert(vm.fetch8() == 10);

    assert(vm.fetch() == core::Opcode::LOAD_STR);
    assert(vm.fetch8() == 8);
    assert(vm.fetch8() == 't');
    assert(vm.fetch8() == 's');
    assert(vm.fetch8() == 't');
    assert(vm.fetch8() == 0);
}

static void test_memory_step()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Strs strs(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());
    assert(vm.regs().pc() == 12);
};

static void test_memory_exe()
{
    core::VM vm((uint8_t*)mem2, sizeof(mem2));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Strs strs(&vm);

    assert(vm.step());
    assert(vm.step());
    assert(vm.step());

    assert(vm.regs().pc() == 12);
    assert(vm.regs().get_string(8) == "abc");

    assert(!vm.step());
    assert(vm.regs().pc() == 13);
    assert(vm.regs().get_string(8) == "abc");
}

static void test_memory_limits()
{
    core::VM vm((uint8_t*)mem3, sizeof(mem3));
    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Strs strs(&vm);

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

static void test_heap_add()
{
    core::VM vm;

    assert(!vm.is_heap(0));
    assert(!vm.is_heap(1));
    assert(!vm.is_heap(10));
    assert(!vm.is_heap(11));

    vm.add_heap(10);
    assert(vm.is_heap(0));
    assert(vm.is_heap(1));
    assert(vm.is_heap(9));
    assert(!vm.is_heap(10));
    assert(!vm.is_heap(11));
}

static void test_heap_add_double()
{
    core::VM vm;

    vm.add_heap(10);
    assert(vm.heap_size() == 10);

    vm.add_heap(10);
    assert(vm.heap_size() == 20);
}

static void test_heap_access()
{
    core::VM vm;

    vm.add_heap(10);

    assert(vm.get_heap(5) == 0);
    vm.heap(5)[5] = 55;
    assert(vm.get_heap(5) == 55);

    assert(vm.get_heap(6) == 0);
    vm.set_heap(6, 42);
    assert(vm.get_heap(6) == 42);

    core::Heap &h = vm.heap(5);
    h[8] = 4;
    assert(vm.get_heap(8) == 4);
    assert(h[6] == 42);

    assertThrows(
        std::string,
        "Heap memory access out of bounds",
        h[10]);
}

static void test_memory_access()
{
    core::VM vm((uint8_t*)mem1, sizeof(mem1));

    vm.add_heap(10);

    assert(vm.mem(0) == (uint8_t)core::Opcode::LOAD_INT8);
    assert(vm.mem(1) == 0);
    assert(vm.mem(2) == 5);

    vm.set_mem(15, 0x42);
    assert(vm.mem(15) == 0x42);

    vm.set_mem(12, 0x40);
    assert(vm.mem(12) == 0x40);

    assertThrows(
        std::string,
        "Write attempt to read only memory",
        vm.set_mem(1, 1));

    assertThrows(
        std::string,
        "Write attempt to read only memory",
        vm.set_mem(10, 0x38));

    assertThrows(
        std::string,
        "Invalid heap access",
        vm.set_mem(30, 0x77));
}

void test_vm()
{
    TEST_CASE(test_basic_opcodes);
    TEST_CASE(test_empty_vm);
    TEST_CASE(test_memory_fetch);
    TEST_CASE(test_memory_step);
    TEST_CASE(test_memory_exe);
    TEST_CASE(test_memory_limits);
    TEST_CASE(test_heap_add);
    TEST_CASE(test_heap_add_double);
    TEST_CASE(test_heap_access);
    TEST_CASE(test_memory_access);
}

#include "framework.hh"
#include <core/heap.hh>
#include <impl/heap.hh>
#include <impl/ints.hh>

static void test_heap_basic()
{
    core::Heap h(5, 10);

    assert(h.pos() == 5);
    assert(h.size() == 10);

    assert(!h.valid(0));
    assert(!h.valid(3));
    assert(h.valid(5));
    assert(h.valid(10));
    assert(h.valid(14));
    assert(!h.valid(15));
    assert(!h.valid(16));
}

static void test_heap_access()
{
    core::Heap h(5, 10);
    core::Heap h2(0, 5);

    assert(h.pos() == 5);
    assert(h.size() == 10);

    assert(h[5] == 0);
    assert(h[6] == 0);
    assert(h[7] == 0);
    assert(h[8] == 0);
    assert(h[9] == 0);
    assert(h[10] == 0);
    assert(h[11] == 0);
    assert(h[12] == 0);
    assert(h[13] == 0);
    assert(h[14] == 0);
    h[6] = 42;
    assert(h[6] == 42);

    h2[0] = 42;
    assert(h2[0] == 42);
}

static void test_heap_access_exception()
{
    core::Heap h(5, 10);
    core::Heap h2(0, 5);

    h[14] = 12;
    assertThrows(
        std::string,
        "Heap memory access out of bounds",
        h[15] = 12);
    assertThrows(
        std::string,
        "Heap memory access out of bounds",
        h[0] = 1);

    h2[0] = 1;
    assert(h2[0] == 1);
}

static void test_heap_add()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::LOAD_INT8, 0, 12,
        (uint8_t)core::Opcode::HEAP, 0,
        (uint8_t)core::Opcode::HEAP, 0,
    };

    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Ints ints(&vm);
    impl::Heap heaps(&vm);

    assert(vm.step());

    assert(vm.heap_size() == 0);

    assert(vm.step());
    assert(vm.heap_size() == 12);

    assert(vm.step());
    assert(vm.heap_size() == 12 * 2);
}

static void test_heap_info()
{
    static uint8_t mem[] = {
        (uint8_t)core::Opcode::INFO, 0, (uint8_t)core::Info::Info,
        (uint8_t)core::Opcode::INFO, 0, (uint8_t)core::Info::Ticks,
        (uint8_t)core::Opcode::INFO, 0, (uint8_t)core::Info::HeapSize,
        (uint8_t)core::Opcode::INFO, 0, (uint8_t)core::Info::HeapSize,
        (uint8_t)core::Opcode::INFO, 0, (uint8_t)core::Info::HeapStart,
        (uint8_t)core::Opcode::INFO, 0, (uint8_t)core::Info::Ticks,
        (uint8_t)core::Opcode::INFO, 0, 0xff
    };
    core::VM vm((uint8_t*)mem, sizeof(mem));
    impl::Heap heaps(&vm);

    // Info
    assert(vm.regs().get_int(0) == 0);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0xdeadbeef);

    // Ticks
    assert(vm.step());
    assert(vm.regs().get_int(0) == 2);

    // HeapSize
    assert(vm.step());
    assert(vm.regs().get_int(0) == 0);

    // HeapSize
    vm.add_heap(20);
    assert(vm.step());
    assert(vm.regs().get_int(0) == 20);

    // HeapStart
    assert(vm.step());
    assert(vm.regs().get_int(0) == sizeof(mem));
    assert(vm.regs().get_int(0) == 3 * 7);

    // Ticks
    assert(vm.step());
    assert(vm.regs().get_int(0) == 6);

    // 0xff
    assertThrows(
        std::string,
        "Unimplemented info entry: 255",
        vm.step());
}

void test_heap()
{
    TEST_CASE(test_heap_basic);
    TEST_CASE(test_heap_access);
    TEST_CASE(test_heap_access_exception);

    TEST_CASE(test_heap_add);
    TEST_CASE(test_heap_info);
}

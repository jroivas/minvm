#include "framework.hh"
#include <heap.hh>

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

void test_heap()
{
    TEST_CASE(test_heap_basic);
    TEST_CASE(test_heap_access);
    TEST_CASE(test_heap_access_exception);
}

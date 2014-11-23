#include "framework.hh"
#include <regs.hh>


static void test_basic_int()
{
    core::Registers regs;
    regs.store_int(0, 1);
    regs.store_int(1, 10);
    regs.store_int(5, 12345871283);

    assert(regs.load_int(0) == 1);
    assert(regs.load_int(1) == 10);
    assert(regs.load_int(5) == 12345871283);
}

static void test_basic_float()
{
    core::Registers regs;
    regs.store_float(0, 1.0);
    regs.store_float(1, 10.1);
    regs.store_float(5, 12345871283.9);

    assert(regs.load_float(0) == 1.0);
    assert(regs.load_float(1) == 10.1);
    assert(regs.load_float(5) == 12345871283.9);
}

static void test_basic_string()
{
    core::Registers regs;
    regs.store_string(0, "1");
    regs.store_string(1, "10");
    regs.store_string(5, "12345871283");
    regs.store_string(9, "abcdefg");

    std::string res("abcdefg");
    res.push_back('\0');
    res += "aa\n88";

    regs.store_string(8, res);

    assert(regs.load_string(0) == "1");
    assert(regs.load_string(1) == "10");
    assert(regs.load_string(5) == "12345871283");
    assert(regs.load_string(9) == "abcdefg");
    assert(regs.load_string(8) == res);
}

static void test_invalid_register()
{
    core::Registers regs;

    regs.store_int(0, 1);
    assert(regs.load_int(0) == 1);

    // 99 should be invalid register
    assertThrows(
        std::string,
        "Invalid register",
        regs.store_int(99, 42));

    assertThrows(
        std::string,
        "Invalid register",
        regs.load_int(99));
}

static void test_type()
{
    core::Registers regs;

    // Default type is int
    assert(regs.type(10) == core::RegisterType::Integer);

    regs.store_int(0, 1);
    regs.store_float(1, 10.1);
    regs.store_string(2, "abc");

    assert(regs.type(0) == core::RegisterType::Integer);
    assert(regs.type(1) == core::RegisterType::Float);
    assert(regs.type(2) == core::RegisterType::String);

    regs.store_int(1, 42);
    assert(regs.type(0) == core::RegisterType::Integer);
    assert(regs.type(1) == core::RegisterType::Integer);
    assert(regs.type(2) == core::RegisterType::String);

    regs.store_float(0, 42.42);
    assert(regs.type(0) == core::RegisterType::Float);
    assert(regs.type(1) == core::RegisterType::Integer);
    assert(regs.type(2) == core::RegisterType::String);
}

static void test_pc()
{
    core::Registers regs;

    assert(regs.pc() == 0);

    regs.next();
    assert(regs.pc() == 1);

    regs.next();
    assert(regs.pc() == 2);

    regs.pc_update(5);
    assert(regs.pc() == 5);

    regs.pc_reset();
    assert(regs.pc() == 0);

    regs.next();
    assert(regs.pc() == 1);

    // PC accessible as int thorough special register -1
    assert(regs.load_int(-1) == 1);

    regs.store_int(-1, 42);
    assert(regs.pc() == 42);

    assertThrows(
        std::string,
        "Invalid register",
        regs.store_float(-1, 44.1));
    assertThrows(
        std::string,
        "Invalid register",
        regs.load_string(-1));
}

void test_regs()
{
    TEST_CASE(test_basic_int);
    TEST_CASE(test_basic_float);
    TEST_CASE(test_basic_string);

    TEST_CASE(test_type);
    TEST_CASE(test_invalid_register);

    TEST_CASE(test_pc);
}

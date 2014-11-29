#include "framework.hh"
#include <regs.hh>


static void test_basic_int()
{
    core::Registers regs;
    regs.put_int(0, 1);
    regs.put_int(1, 10);
    regs.put_int(5, 12345871283);

    assert(regs.get_int(0) == 1);
    assert(regs.get_int(1) == 10);
    assert(regs.get_int(5) == 12345871283);
}

static void test_basic_float()
{
    core::Registers regs;
    regs.put_float(0, 1.0);
    regs.put_float(1, 10.1);
    regs.put_float(5, 12345871283.9);

    assert(regs.get_float(0) == 1.0);
    assert(regs.get_float(1) == 10.1);
    assert(regs.get_float(5) == 12345871283.9);
}

static void test_basic_string()
{
    core::Registers regs;
    regs.put_string(0, "1");
    regs.put_string(1, "10");
    regs.put_string(5, "12345871283");
    regs.put_string(9, "abcdefg");

    std::string res("abcdefg");
    res.push_back('\0');
    res += "aa\n88";

    regs.put_string(8, res);

    assert(regs.get_string(0) == "1");
    assert(regs.get_string(1) == "10");
    assert(regs.get_string(5) == "12345871283");
    assert(regs.get_string(9) == "abcdefg");
    assert(regs.get_string(8) == res);
}

static void test_invalid_register()
{
    core::Registers regs;

    regs.put_int(0, 1);
    assert(regs.get_int(0) == 1);

    // 99 should be invalid register
    assertThrows(
        std::string,
        "Invalid register",
        regs.put_int(99, 42));

    assertThrows(
        std::string,
        "Invalid register",
        regs.get_int(99));
}

static void test_type()
{
    core::Registers regs;

    // Default type is int
    assert(regs.type(10) == core::RegisterType::Integer);

    regs.put_int(0, 1);
    regs.put_float(1, 10.1);
    regs.put_string(2, "abc");

    assert(regs.type(0) == core::RegisterType::Integer);
    assert(regs.type(1) == core::RegisterType::Float);
    assert(regs.type(2) == core::RegisterType::String);

    regs.put_int(1, 42);
    assert(regs.type(0) == core::RegisterType::Integer);
    assert(regs.type(1) == core::RegisterType::Integer);
    assert(regs.type(2) == core::RegisterType::String);

    regs.put_float(0, 42.42);
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
    assert(regs.get_int(-1) == 1);

    regs.put_int(-1, 42);
    assert(regs.pc() == 42);

    assertThrows(
        std::string,
        "Invalid register",
        regs.put_float(-1, 44.1));
    assertThrows(
        std::string,
        "Invalid register",
        regs.get_string(-1));
}

static void test_copy()
{
    core::Registers regs;

    regs.put_int(0, 1);
    regs.put_float(1, 10.1);
    regs.put_string(2, "abc");

    assert(regs.type(5) == core::RegisterType::Integer);
    assert(regs.get_int(5) == 0);
    assert(regs.type(6) == core::RegisterType::Integer);
    assert(regs.type(7) == core::RegisterType::Integer);

    regs.copy(5, 0);
    regs.copy(6, 1);
    regs.copy(7, 2);

    assert(regs.type(5) == core::RegisterType::Integer);
    assert(regs.get_int(5) == 1);
    assert(regs.get_int(0) == 1);
    assert(regs.type(6) == core::RegisterType::Float);
    assert(regs.get_float(6) == 10.1);
    assert(regs.get_float(1) == 10.1);
    assert(regs.type(7) == core::RegisterType::String);
    assert(regs.get_string(7) == "abc");
    assert(regs.get_string(2) == "abc");

    regs.put_int(0, 42);
    assert(regs.get_int(5) == 1);
    assert(regs.get_int(0) == 42);

    regs.copy(5, 7);
    assert(regs.type(5) == core::RegisterType::String);
    assert(regs.get_string(5) == "abc");
    assert(regs.get_string(7) == "abc");
    assert(regs.get_string(2) == "abc");

    regs.put_string(7, "test");
    assert(regs.get_string(5) == "abc");
    assert(regs.get_string(7) == "test");
    assert(regs.get_string(2) == "abc");
}

void test_regs()
{
    TEST_CASE(test_basic_int);
    TEST_CASE(test_basic_float);
    TEST_CASE(test_basic_string);

    TEST_CASE(test_type);
    TEST_CASE(test_invalid_register);

    TEST_CASE(test_pc);
    TEST_CASE(test_copy);
}

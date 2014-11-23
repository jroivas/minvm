#include "framework.hh"
#include <vector>
#include <atomic>
#include <map>
#include <unistd.h>

static std::map<std::string, std::function<void(void)>> _test_cases;
static std::atomic<unsigned int> __success_cnt;
static std::atomic<unsigned int> __fail_cnt;
static std::atomic<unsigned int> __suites;
static std::atomic<unsigned int> __case_cnt;
static std::atomic<unsigned int> __exceptions;

void register_test_function(std::string name, std::function<void(void)> test)
{
    _test_cases[name] = test;
}

static int color_normal = 39;
static int color_green = 32;
static int color_red = 31;

std::string output_color(int color)
{
    return "\033[" + std::to_string(color) + "m";
}

unsigned int run_tests()
{
    for (auto test : _test_cases) {
        ++__suites;
        unsigned int pre_asserts = __success_cnt + __fail_cnt;
        unsigned int pre_success = __success_cnt;
        unsigned int pre_fail_asserts = __fail_cnt;
        unsigned int pre_cases = __case_cnt;

        std::cout << output_color(color_normal)
                  << "*** RUNNING: "
                  << test.first
                  << std::flush;

        unsigned int pre_exceptions = __exceptions;

        try {
            test.second();
        }
        catch (std::string e) {
            std::cout << "!";
            ++__fail_cnt;
            ++__exceptions;
            std::cerr << output_color(color_red)
                      << "\nEXCEPTION: " << e
                      << output_color(color_normal)
                      << "\n";
        }

        std::cout << std::endl;
        unsigned int post_asserts = __success_cnt + __fail_cnt;
        unsigned int post_cases = __case_cnt - pre_cases;
        unsigned int post_exceptions = __exceptions - pre_exceptions;
        unsigned int fail_diff = __fail_cnt - pre_fail_asserts;
        unsigned int post_success = __success_cnt - pre_success;
        std::cout << "    "
                  << post_cases
                  << " cases"
                  << std::endl;
        std::cout << "    "
                  << post_asserts - pre_asserts
                  << " asserts" << std::endl;
        if (post_success > 0) {
            std::cout << output_color(color_green)
                      << "    "
                      << post_success
                      << " successful" << std::endl;
        }
        if (fail_diff > 0) {
            std::cout << output_color(color_red)
                      << "    "
                      << fail_diff
                      << " FAILED"
                      << std::endl;
        }
        if (post_exceptions > 0) {
            std::cout << "    "
                      << output_color(color_red)
                      << post_exceptions
                      << " EXCEPTION"
                      << std::endl;
        }
        std::cout << output_color(color_normal);
    }

    return __fail_cnt;
}

void success()
{
    ++__success_cnt;
    std::cout << "." << std::flush;
}

void fail()
{
    ++__fail_cnt;
    std::cout << "x" << std::flush;
}

void register_case()
{
    ++__case_cnt;
}

std::string test_stats()
{
    std::string res = "";
    res += "------------------\n";
    res += "Test suites run  : " + std::to_string(__suites) + "\n";
    res += "Test cases run   : " + std::to_string(__case_cnt) + "\n";
    res += "Successful tests : " + std::to_string(__success_cnt) + "\n";
    res += "Failed tests     : " + std::to_string(__fail_cnt) + "\n";
    res += "Exceptions       : " + std::to_string(__exceptions) + "\n";
    res += "Total tests      : " + std::to_string(__fail_cnt + __success_cnt)+ "\n";
    return res;
}

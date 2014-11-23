#include <iostream>

#include "framework.hh"

int main(int argc, char **argv)
{
    REGISTER_TEST(regs);


    unsigned int res = 0;
    try {
        res = run_tests();
    }
    catch (std::string e) {
        std::cout << "\n!!! ERROR: " << e << "\n";
        return 1;
    }

    std::cout << test_stats() << std::endl;

    return res;
}

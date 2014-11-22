#include <iostream>
#include <fstream>
#include <cstdint>

#include "opcodes.hh"
#include "vm.hh"
#include "asm1/asm1.hh"

using namespace core;

static uint8_t code[] = {
    (uint8_t)Opcode::STORE_INT8, 0, 4,
    (uint8_t)Opcode::STORE_INT32, 1, 0x12, 0x34, 0x56, 0x78,
    (uint8_t)Opcode::ADD_INT, 2, 0, 1,
    (uint8_t)Opcode::PRINT_INT, 2,
    (uint8_t)Opcode::STORE_STR, 1,
    '\n', 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n', 0,
    (uint8_t)Opcode::PRINT_STR, 1,
    (uint8_t)Opcode::STOP
};

int main(int argc, char **argv)
{
    VM vm((uint8_t*)code);
    //vm.set_debug();
    ASM1 inst(&vm);

    try {
        while (vm.step());
    }
    catch (std::string e) {
        std::cerr << "\n*** EXCEPTION: " << e << "\n";
        return 1;
    }
#if 0
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " application\n";
        return 1;
    }

    std::ifstream input(argv[1]);
#endif
    return 0;
}

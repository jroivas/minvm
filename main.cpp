#include <iostream>
#include <fstream>
#include <cstdint>

#include "opcodes.hh"
#include "vm.hh"
#include "impl/nopstop.hh"
#include "impl/ints.hh"
#include "impl/strs.hh"
#include "impl/random.hh"
#include "impl/jump.hh"
#include "impl/mov.hh"
#include "impl/heap.hh"

using namespace core;

int main(int argc, char **argv)
{
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " application\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::in | std::ios::binary);
    std::string code(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>());

    VM vm((uint8_t*)code.data(), code.length());
    //vm.set_debug();

    impl::NopStop nopstop(&vm);
    impl::Ints ints(&vm);
    impl::Strs strs(&vm);
    impl::Random rand(&vm);
    impl::Jump jump(&vm);
    impl::Mov mov(&vm);
    impl::Heap heap(&vm);

    try {
        while (vm.step());
    }
    catch (std::string e) {
        std::cerr << "\n*** EXCEPTION: " << e << "\n";
        std::cerr << "\n" << vm.regs().dump();
        return 1;
    }
    return 0;
}

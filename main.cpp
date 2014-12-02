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

#include <map>
#include <string>

void usage(std::string app)
{
    std::cout << "Usage: " << app << " application\n";
    std::cout << "  -h|--help      This help\n";
    std::cout << "  -d|--debug     Set debug\n";
}

std::map<std::string, std::string> parseArgs(int argc, char **argv)
{
    bool got_app = false;
    std::map<std::string, std::string> res;
    for (int i = 1; i < argc; ++i) {
        std::string val = argv[i];
        if (val == "-d" ||
            val == "--debug") {
            res["debug"] = "true";
        } else if (val == "-h" ||
            val == "--help") {
            usage(argv[0]);
            exit(0);
        } else if (val[0] != '-') {
            if (got_app) {
                std::cout << "\nERROR: Application already defined,"
                    << " invalid argument: " << val << "\n\n";
                usage(argv[0]);
                exit(1);
            }
            got_app = true;
            res["fname"] = val;
        } else {
            std::cout << "\nERROR: Invalid arugment: " << val << "\n\n";
            usage(argv[0]);
            exit(1);
        }
    }
    return res;
}

int main(int argc, char **argv)
{
    std::map<std::string, std::string> args = parseArgs(argc, argv);
    auto fname = args.find("fname");
    if (fname == args.end()) {
        std::cout << "\nERROR: Missing application!\n\n";
        usage(argv[0]);
        return 1;
    }

    std::ifstream input(fname->second, std::ios::in | std::ios::binary);
    std::string code(
        (std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>());

    VM vm((uint8_t*)code.data(), code.length());
    auto debug = args.find("debug");
    if (debug != args.end())
        vm.set_debug();

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

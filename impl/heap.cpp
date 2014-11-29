#include "heap.hh"
#include "opcodes.hh"
#include <iostream>

using core::VM;
using core::Opcode;
using impl::Heap;

Heap::Heap(VM *vm)
{
    vm->opcode((uint32_t)Opcode::HEAP, Heap::heap);
    vm->opcode((uint32_t)Opcode::INFO, Heap::info);
}

bool Heap::heap(core::VM *vm)
{
    if (vm->debug()) std::cerr << "HEAP\n";

    uint8_t reg = vm->fetch8();
    uint64_t amount = vm->regs().get_int(reg);

    vm->add_heap(amount);

    return true;
}

bool Heap::info(core::VM *vm)
{
    if (vm->debug()) std::cerr << "INFO\n";

    uint8_t reg1 = vm->fetch8();
    uint8_t reg2 = vm->fetch8();

    uint64_t val = 0;

    switch ((core::Info)reg2) {
        case core::Info::Info:
            val = 0xdeadbeef;
            break;
        case core::Info::Ticks:
            val = vm->ticks();
            break;
        case core::Info::HeapSize:
            val = vm->heap_size();
            break;
        case core::Info::HeapStart:
            val = vm->size();
            break;
        default:
            throw std::string("Unimplemented info entry: "
                + std::to_string(reg2));
    }

    vm->regs().put_int(reg1, val);

    return true;
}

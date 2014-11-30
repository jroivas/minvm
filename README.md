# MinVM

Simple imaginary VM.

Copyright (c) 2014 Jouni Roivas <jroivas@iki.fi>


## Design

Designed to be easily extended. Framework itself does not make assumptions about the architecture.
Only restriction is that first character is OPCODE. Opcodes are 8 bit.
Opcodes may have varying amount of parameters and can be dependant of the opcode.

Different opcode handlers can be implemented separately. Custom opcodes can be easily added.

When opcode is met, the handler is searched from opcode table. Table contains opcode mapped to a handler function.
Default handler just throws exception, so used opcodes must be explicitly implemented.
Pointer to current VM is given as a parameter to handler, so it can access all registers and memory.
Implementation of next bytes depends on opcode. It can have from zero to any amount of arguments.
Current implementation has variable length opcodes, but nothing prevents from implementing fixed length opcodes.

Every handler function should return boolean value. Return value determines if execution should be stopped.


## Building

You need C++11 capable compiler and CMake. Python needed for assembler.

    mkdir build
    cd build
    cmake ..
    make
    make test


## Assembler

Assembler is provided to translate symbolic programs (see examples/ folder) to executable binary.
Requires Python 2.7+

Supports only subset of MinVM features, but suits already for many cases. Target is to improve it
gradually full blown tool.


## License

MIT

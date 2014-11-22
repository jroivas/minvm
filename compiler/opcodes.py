# Generated opcodes
# Do not update manually
# Use: ./compiler/opcode_gen.py core/opcodes.hh

NOP = 0
LOAD_INT = 1
STORE_INT8 = 2
STORE_INT16 = 3
STORE_INT32 = 4
STORE_INT64 = 5
LOAD_STR = 6
STORE_STR = 7
INC_INT = 8
DEC_INT = 9
ADD_INT = 10
SUB_INT = 11
MUL_INT = 12
DIV_INT = 13
MOD_INT = 14
PRINT_INT = 15
PRINT_FLOAT = 16
PRINT_STR = 17
RANDOM = 18
JMP8 = 19
JMP16 = 20
JMP32 = 21
JMP64 = 22
JMP_INT = 23
JMP_LE8 = 24
JMP_LE16 = 25
JMP_LE32 = 26
JMP_LE64 = 27
JMP_LE_INT = 28
STOP = 255
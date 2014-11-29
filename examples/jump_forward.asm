LOAD R0, 1234
LOAD R1, 0
LOAD R2, 20
LOAD R3, 42

LOAD R9, "\n"
LOAD R11, "One should not see this!"
LOAD R12, "Result should be 42: "
LOAD R14, "OK"
LOAD R15, "Fail"

loop:
    INC R1
    JMP R1 < R2, loop

ADD R1, R1, R1
INC R1
JMP R1 > R2, next

PRINT R11
PRINT R9

next: INC R1

PRINT R12
PRINT R1
PRINT R9

JMP R1 != R3, fail

PRINT R14
PRINT R9

STOP

fail:
PRINT R15
PRINT R9

STOP

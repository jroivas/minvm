; Loop 0..20

STORE R1, 0
STORE R2, 20
STORE R9, "\n"

begin:
    PRINT R1
    PRINT R9
    INC R1
    JMP R1 < R2, begin

STOP

endline: DB '\n'

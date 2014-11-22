; Loop 0..20

STORE R1, 0
STORE R2, 20

begin:
    PRINT R1
    PRINT endline
    INC R1
    JMP R1 < R2, begin

STOP

endline: DB '\n'

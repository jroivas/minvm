; Loop 0..20

LOAD R1, 0
LOAD R2, 20

begin:
    PRINT R1
    PRINT endline
    INC R1
    JMP R1 < R2, begin

STOP

endline: DB '\n'

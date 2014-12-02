; Number to get sqrt
LOAD R0, 1764

; Other variables
LOAD R11, 0
LOAD R12, 1
LOAD R8, -1
LOAD R9, 2
MOV R3, R11
MOV R2, R0

; Newtons method for square root
sqrt:
    ; R1 = R2
    MOV R1, R2

    ; R2 = (R1 + R0/R1) / R9
    DIV R2, R0, R1
    ADD R2, R2, R1
    DIV R2, R2, R9

    SUB R3, R1, R2
    JMP R3 >= R11, sqrt_end

    MUL R3, R3, R8

sqrt_end:
    JMP R3 > R12, sqrt

; Print result
LOAD R10, "\n"
PRINT R2
PRINT R10
STOP

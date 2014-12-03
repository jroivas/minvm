; Number to get sqrt
LOAD R0, 1764

; Other variables
LOAD R3, 0
MOV R2, R0

; Newtons method for square root
sqrt:
    ; R1 = R2
    MOV R1, R2

    ; R2 = (R1 + R0/R1) / 2
    DIV R2, R0, R1
    ADD R2, R2, R1
    DIV R2, R2, 2

    SUB R3, R1, R2
    JMP R3 >= 0, sqrt_end

    MUL R3, R3, -1

sqrt_end:
    JMP R3 > 1, sqrt

; Print result
LOAD R10, "\n"
PRINT R2
PRINT R10
STOP


LOAD R1, 1, [data]
LOAD R2, 2, [data]
LOAD R3, 3, [data]
LOAD R4, 4, [data]

LOAD R5, 4, [data + 3]

LOAD R6, 2, [data2]

LOAD R10, "\n"

PRINT R1
PRINT R10

PRINT R2
PRINT R10

PRINT R3
PRINT R10

PRINT R4
PRINT R10

PRINT R5
PRINT R10

PRINT R6
PRINT R10

STOP


data:
    DB 0x42, 1, 0x55, 2, 4, 5, 6
data2:
    DB 0x99, 0xfe

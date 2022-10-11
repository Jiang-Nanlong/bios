CODE SEGMENT
          ASSUME CS:CODE
    START:
    OCW1  EQU    21H
          IN     AL,OCW1
          OR     AL,00000010B
    ;serial port1 00001000B
    ;serial port2 00010000B
          OUT    OCW1,AL
CODE ENDS
END START
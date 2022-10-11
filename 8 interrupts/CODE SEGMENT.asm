CODE SEGMENT
            ASSUME CS:CODE
      START:
      L1:   
            MOV    AH,0
            INT    16H        ;Keyboard input, at this time AH stores the scan code
            CMP    AH,	1     ;The scan code of esc is 1
            JE     L2         ;if it is esc
            MOV    DL,AH      ;Output scan code
            MOV    AH,02H
            INT    21H
            JNE    L1         ;Continue the loop
      L2:   
            MOV    AX, 4C00H  ;End the program
            INT    21H
CODE ENDS
END START
.MODEL SMALL
.STACK 100H
.DATA
    ; No data needed
.CODE
MAIN PROC
               MOV  BX, 0A7CCH    ; Load test value into BX
    
               MOV  AX, 0         ; Init count in AX
               MOV  CX, 16        ; Set loop for 16 bits
    
    COUNT_LOOP:
               ROL  BX, 1         ; Rotate BX left
               JNC  NO_CARRY      ; Skip if no carry
               INC  AX            ; Inc count if carry
    NO_CARRY:  
               LOOP COUNT_LOOP    ; Loop 16 times
    
    ; AX has bit count
    ; Display count (0-9)
               ADD  AL, '0'       ; To ASCII
               MOV  DL, AL        ; To DL
               MOV  AH, 2         ; DOS char output
               INT  21H           ; Display
    
    ; Exit
               MOV  AH, 4CH
               INT  21H
MAIN ENDP
END MAIN
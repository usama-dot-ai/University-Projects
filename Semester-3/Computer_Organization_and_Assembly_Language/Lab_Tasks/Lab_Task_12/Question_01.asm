; Single Digit Output
.MODEL SMALL
.STACK 100H

.DATA
    PROMPT     DB 'Enter a number (0-9): $'
    RESULT_MSG DB 0DH, 0AH, 'Result multiplied by 2: $'
    NEWLINE    DB 0DH, 0AH, '$'
    INPUT_NUM  DB ?
    OUTPUT_NUM DW ?

.CODE
MAIN PROC
                   MOV  AX, @DATA
                   MOV  DS, AX

    ; Display prompt
                   MOV  AH, 09H
                   LEA  DX, PROMPT
                   INT  21H

    ; Read single digit input
                   MOV  AH, 01H
                   INT  21H
                   SUB  AL, '0'           ; Convert ASCII to binary
                   MOV  INPUT_NUM, AL     ; Store input

    ; Clear AH for 16-bit operations
                   MOV  AH, 0

    ; Multiply by 2 using SHL (Shift Left)
    ; SHL shifts bits left, effectively multiplying by 2^n
    ; SHL AL, 1 = Multiply by 2^1 = Multiply by 2
                   MOV  AL, INPUT_NUM
                   SHL  AL, 1             ; AL = AL * 2
                   MOV  OUTPUT_NUM, AX    ; Store result

    ; Display result message
                   MOV  AH, 09H
                   LEA  DX, RESULT_MSG
                   INT  21H

    ; Display result
                   MOV  AX, OUTPUT_NUM
                   CALL DISPLAY_NUMBER

    ; Exit program
                   MOV  AH, 4CH
                   INT  21H
MAIN ENDP

    ; Subroutine to display number (0-18)
DISPLAY_NUMBER PROC
    ; Check if number is two-digit
                   CMP  AL, 10
                   JL   SINGLE_DIGIT

    ; Two-digit number (10-18)
                   MOV  BL, 10            ; Ensure this is 10, not 8
                   DIV  BL                ; AL = quotient (tens), AH = remainder (ones)

    ; Display tens digit
                   MOV  DL, AL
                   ADD  DL, '0'
                   MOV  AH, 02H
                   INT  21H

    ; Display ones digit
                   MOV  DL, AH
                   ADD  DL, '0'
                   INT  21H
                   RET

    SINGLE_DIGIT:  
    ; Single digit number (0-9)
                   ADD  AL, '0'
                   MOV  DL, AL
                   MOV  AH, 02H
                   INT  21H
                   RET
DISPLAY_NUMBER ENDP

END MAIN
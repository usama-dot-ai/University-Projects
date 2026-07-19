.model small
.stack 100h

.code
main proc

               CMP  AX, 5         ; Compare AX with 0 (corrected from -1)
               JL   NEGATIVE      ; If AX < 0, jump to NEGATIVE
               JE   ZERO          ; If AX == 0, jump to ZERO
               JG   POSITIVE      ; If AX > 0, jump to POSITIVE

    NEGATIVE:  
               MOV  BX, -1        ; Set BX to -1
               JMP  END_CASE      ; Jump to end

    ZERO:      
               MOV  BX, 0         ; Set BX to 0
               JMP  END_CASE      ; Jump to end

    POSITIVE:  
               MOV  BX, 1         ; Set BX to 1
               JMP  END_CASE      ; Jump to end

    END_CASE:  
    ; Display BX on the screen (assuming DOS INT 21h for simplicity)
    ; Convert BX to a string and print it
    ; For brevity, using a simple method to print the value as a signed integer

    ; Prepare to print
               MOV  AH, 02H       ; DOS function to display character
               CMP  BX, 0
               JL   PRINT_NEG     ; If negative, print '-' first
               JE   PRINT_ZERO    ; If zero, print '0'
    ; Else positive, print '+' and then digit

    PRINT_POS: 
               MOV  DL, '+'       ; Print '+'
               INT  21H
               MOV  AX, BX        ; Move BX to AX for processing
               CALL PRINT_NUM     ; Call subroutine to print number
               JMP  DONE

    PRINT_NEG: 
               MOV  DL, '-'       ; Print '-'
               INT  21H
               MOV  AX, BX
               NEG  AX            ; Make positive for printing
               CALL PRINT_NUM
               JMP  DONE

    PRINT_ZERO:
               MOV  DL, '0'       ; Print '0'
               INT  21H
               JMP  DONE

    PRINT_NUM:                    ; Subroutine to print AX as unsigned digit (assuming small number)
               ADD  AL, '0'       ; Convert to ASCII (for single digit, e.g., 1 -> '1')
               MOV  DL, AL
               MOV  AH, 02H
               INT  21H
               RET

    DONE:      
    ; Exit the program
               MOV  AH, 4CH       ; DOS function to terminate program
               INT  21H

main endp
end main
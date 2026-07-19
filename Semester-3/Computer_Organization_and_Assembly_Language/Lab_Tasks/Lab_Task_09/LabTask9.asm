.MODEL SMALL
.STACK 100H

; Two extended ASCII chars: C9H (201 '╔') and CDH (205 '═')

.CODE
MAIN PROC
        
    ; Load chars into AL and BL
              MOV  AL, 201
              MOV  BL, 205

    ; Compare AL and BL
              CMP  AL, BL

    ; If AL > BL, jump to display_B
              JNBE display_B

    ; Display AL if AL <= BL
    display_A:
              MOV  DL, AL
              JMP  end_if

    ; Display BL if AL > BL
    display_B:
              MOV  DL, BL

    end_if:   
    ; Display char in DL
              MOV  AH, 02H
              INT  21H

    ; Exit program
              MOV  AH, 4CH
              INT  21H
MAIN ENDP
END MAIN
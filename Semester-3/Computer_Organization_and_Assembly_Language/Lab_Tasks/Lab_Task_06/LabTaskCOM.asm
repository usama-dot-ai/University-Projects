; COM program to display a prompt, read a char, and newline
ORG 100H

START:
    ; Display prompt character '>'
    MOV AH, 02H
    MOV DL, 'Enter a Character >'
    INT 21H

    ; Read a character from keyboard (stored in AL, also echoed)
    MOV AH, 01H
    INT 21H

    ; Move to new line: CR (0DH) and LF (0AH)
    MOV AH, 02H
    MOV DL, 0DH
    INT 21H
    MOV DL, 0AH
    INT 21H

    ; Terminate program
    MOV AH, 4CH
    INT 21H

    END START
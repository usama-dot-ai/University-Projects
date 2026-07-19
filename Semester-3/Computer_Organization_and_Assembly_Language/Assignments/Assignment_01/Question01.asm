.MODEL TINY
.CODE
          ORG 100h

    START:
          MOV AL, 05h    ; First number (5)
          ADD AL, 03h    ; Add second number (3)
          ADD AL, 30h    ; Convert to ASCII digit
    
          MOV DL, AL     ; Prepare for output
          MOV AH, 02h    ; Output character to STDOUT
          INT 21h        ; Interrupt to DOS
    
          MOV AH, 4Ch    ; Terminate program
          INT 21h        ; Exit to DOS

    END START
    
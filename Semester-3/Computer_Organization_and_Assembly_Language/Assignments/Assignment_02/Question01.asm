.MODEL SMALL
.STACK 100h

.DATA
    A    DW 2    ; Define A as a word (16-bit)
    B    DW 5    ; Define B as a word (16-bit)
    SUM  DW ?    ; Uninitialized word for sum

.CODE
MAIN PROC
    ; Initialize data segment
         MOV AX, @DATA
         MOV DS, AX
    
    ; Add the numbers
         MOV AX, A        ; Load A into AX
         ADD AX, B        ; Add B to AX  (Result is now in AX)
    
    ; Prepare to display the result
         MOV AL, AL       ; Ensure we're working with the low byte
         ADD AL, 30h      ; Convert low byte to ASCII
    
         MOV DL, AL       ; Move to DL for output
         MOV AH, 02h      ; Output character
         INT 21h          ; Call DOS interrupt to display
    
    ; Exit to DOS
         MOV AX, 4C00h    ; Terminate with exit code 0
         INT 21h          ; Call DOS interrupt

MAIN ENDP
END MAIN     ; End of program


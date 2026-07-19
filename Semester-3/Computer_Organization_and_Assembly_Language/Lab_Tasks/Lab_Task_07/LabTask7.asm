.MODEL SMALL          ; Define small memory model for the program
.STACK 100H           ; Allocate 256 bytes for the stack

.DATA
    MSG1 DB  'Enter a lowercase letter: $'
    MSG2 DB  0DH,0AH, 'The uppercase letter is: $'
    CR   EQU 0DH                                      ; Define carriage return constant
    LF   EQU 0AH                                      ; Define line feed constant

.CODE
MAIN PROC
              MOV AX, @DATA    ; Load data segment address into AX
              MOV DS, AX       ; Set DS to point to data segment
    
    ; Display prompt message
              LEA DX, MSG1     ; Load effective address of MSG1 into DX
              MOV AH, 09H
              INT 21H
    
    ; Read character from keyboard
              MOV AH, 01H      ; Function to read character with echo
              INT 21H
    
    ; Store the lowercase letter temporarily
              MOV BL, AL       ; Move input character to BL for processing
    
    ; Convert lowercase to uppercase (only if in 'a'..'z')
              CMP BL, 'a'
              JB  NOT_LOWER
              CMP BL, 'z'
              JA  NOT_LOWER
              SUB BL, 20H      ; Subtract 32 to convert to uppercase
    
    NOT_LOWER:                 ; Label for non-lowercase characters
    ; Display result message
              LEA DX, MSG2     ; Load effective address of MSG2 into DX
              MOV AH, 09H
              INT 21H
    
    ; Display the uppercase character
              MOV DL, BL       ; Move converted character to DL
              MOV AH, 02H
              INT 21H
    
    ; Display newline
              MOV DL, CR       ; Load carriage return into DL
              MOV AH, 02H
              INT 21H
              MOV DL, LF       ; Load line feed into DL
              MOV AH, 02H
              INT 21H
    
    ; Exit program
              MOV AH, 4CH
              INT 21H
MAIN ENDP
END MAIN

.MODEL SMALL
.STACK 100h

.DATA
    prompt     DB  "Enter 'Y' or 'y' to display the character: $"
    NEWLINE_CR EQU 13                                                ; Carriage Return ASCII
    NEWLINE_LF EQU 10                                                ; Line Feed ASCII

.CODE
MAIN PROC
    ; Initialize data segment
             MOV  AX, @DATA
             MOV  DS, AX

    ; --- 1. Display Prompt ---
             MOV  AH, 09h           ; DOS interrupt for displaying a string
             LEA  DX, prompt        ; Load effective address of 'prompt'
             INT  21h

    ; --- 2. Read Character (with echo) ---
             MOV  AH, 01h           ; Function 01h: Read character WITH echo (shows input immediately)
             INT  21h               ; AL now contains the input character, and it has been displayed

    ; --- 3. Print a Newline (to move cursor before final output) ---
    ; Note: We save AL before using AH/DL
             PUSH AX                ; Preserve the input character (in AL)
 
             MOV  DL, NEWLINE_CR    ; Carriage return
             MOV  AH, 02h           ; Function 02h: Display character in DL
             INT  21h
 
             MOV  DL, NEWLINE_LF    ; Line feed
             MOV  AH, 02h
             INT  21h
 
             POP  AX                ; Restore the input character back to AL

    ; --- 4. Conditional Logic Check ---

    ; Check if input is 'Y' (uppercase)
             CMP  AL, 'Y'
             JE   ShowChar          ; If equal, jump to ShowChar

    ; Check if input is 'y' (lowercase)
             CMP  AL, 'y'
             JE   ShowChar          ; If equal, jump to ShowChar

    ; If neither 'Y' nor 'y', jump to exit
             JMP  EXIT

    ShowChar:
    ; --- 5. Display the Character (on the new line) ---
             MOV  DL, AL            ; Move the character from AL to DL for output
             MOV  AH, 02h           ; DOS interrupt for writing a character
             INT  21h

    EXIT:    
    ; --- 6. Terminate the program ---
             MOV  AH, 4Ch           ; DOS interrupt for program termination
             INT  21h

MAIN ENDP
END MAIN
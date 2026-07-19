.model small
.386                       ; Required for 32-bit registers or advanced instructions if needed
.stack 100h

.data
    ; Declare two 16-bit values for comparison
    source    DW 500
    target    DW 300

    ; Output messages
    msg_above DB 'Source is greater than Target.$'
    msg_below DB 'Source is not greater than Target.$'

.code
Main PROC
    ; Initialize Data Segment
                    mov   ax, @data
                    mov   ds, ax
    
    ; Ensure correct setup of the ES segment for string instructions
                    mov   es, ax

    ; Load the addresses of source and target into SI and DI
                    lea   si, source
                    lea   di, target

    ; CMPSW compares the word at DS:SI with the word at ES:DI
                    cld
                    cmpsw

    ; Use a conditional jump (JA - Jump if Above/Unsigned Greater)
                    ja    SourceIsGreater

    ; --- ELSE: Source <= Target ---
                    lea   dx, msg_below
                    jmp   DisplayMessage

    SourceIsGreater:
    ; --- IF: Source > Target ---
                    lea   dx, msg_above

    DisplayMessage: 
    ; Display the appropriate message using INT 21h (function 09h)
                    mov   ah, 09h
                    int   21h

    ; Terminate Program
                    mov   ah, 4ch
                    int   21h
Main ENDP
end Main
.model small
.stack 100h
.code

main proc
               mov cx, 26        ; Initialize counter to 26 (for A to Z)
               mov bl, 'A'       ; Start with 'A' in BL (BL holds the current letter code)
    
    loop_start:
  
               mov dl, bl        ; Load current letter into DL
               mov ah, 2
               int 21h           ; Print the current character
    
    ; Print a space after the character
               mov dl, 32        ; ASCII code for space
               mov ah, 2
               int 21h
    
               inc bl            ; Increment letter code in BL
               dec cx            ; Decrement counter
               jnz loop_start    ; Jump back if counter is not zero
    
    ; Terminate the program (AH=4Ch for INT 21H)
               mov ah, 4ch
               int 21h
    
main endp
end main
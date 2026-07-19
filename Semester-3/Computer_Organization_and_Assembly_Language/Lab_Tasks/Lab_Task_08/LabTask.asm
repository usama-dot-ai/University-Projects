.model small
.stack 100h
.code

main proc
    ; Initialize counter to 256 (loop runs 256 times)
               mov cx, 256
    
    ; Start with character code 0 in BL (BL holds the current character code)
               mov bl, 0
    
    print_loop:
    ; Load current character into DL
               mov dl, bl
    
    ; Print the current character (AH=2 for INT 21H)
               mov ah, 2
               int 21h
    
    ; Print a space after the character
               mov dl, 32        ; ASCII code for space
               mov ah, 2
               int 21h
    
    ; Increment character code in BL
               inc bl
    
    ; Decrement counter
               dec cx
    
    ; Jump back if counter is not zero
               jnz print_loop
    
    ; Terminate the program (AH=4Ch for INT 21H)
               mov ah, 4ch
               int 21h
    
main endp
end main
; ------------------------------------------------------------
; Hex/Binary Viewer for Memory Inspection (emu8086 / 8086 DOS)
; UPDATED: Fixed binary output bug (AL preserved between HEX/BIN)
; ------------------------------------------------------------
; Requirements satisfied:
; - 3+ assembly concepts: interrupts (INT 21h), procedures, string ops,
;   arithmetic/bitwise operations
; - User interaction: keyboard input + screen output
; - Modular design using procedures
; - Error handling: invalid hex, invalid length, invalid mode
; - Clear documentation
; ------------------------------------------------------------

.model small
.stack 100h

.data
    ; ----------- messages -----------
    msgTitle     db 13,10,"Hex/Binary Viewer for Memory Inspection",13,10,"$"
    msgOff       db 13,10,"Enter start OFFSET in hex (0000-FFFF): $"
    msgLen       db 13,10,"Enter LENGTH in hex bytes (0001-00FF): $"
    msgMode      db 13,10,"Mode: H=Hex, B=Binary, A=All : $"
    msgErrHex    db 13,10,"ERROR: Invalid hex input. Use 0-9, A-F.",13,10,"$"
    msgErrLen    db 13,10,"ERROR: Length must be 0001-00FF.",13,10,"$"
    msgErrMode   db 13,10,"ERROR: Mode must be H, B, or A.",13,10,"$"
    msgOutHdr    db 13,10,"----- OUTPUT -----",13,10,"$"
    msgPressKey  db 13,10,"Press any key to exit...$"

    ; ----------- input buffers (DOS 0Ah) -----------
    ; Structure: [max][len][chars...]
    bufOff   db 5,0, 5 dup(0)   ; up to 4 hex digits + enter
    bufLen   db 5,0, 5 dup(0)   ; up to 4 hex digits
    bufMode  db 2,0, 2 dup(0)   ; 1 char mode

    ; ----------- working values -----------
    startOff dw 0
    lengthB  dw 0
    mode     db 0               ; 'H', 'B', 'A'

    ; formatting
    nl       db 13,10,"$"
    space    db " $"
    colonsp  db ": $"

    ; demo memory area (optional)
    demoData db 16h, 2Ah, 0FFh, 10h, 55h, 0A0h, 01h, 02h
             db 03h, 04h, 0BCh, 0DEh, 0ADh, 0BEh, 0EFh, 00h

.code

; ------------------- MAIN -------------------
main proc
    mov ax, @data
    mov ds, ax

    ; Show title
    lea dx, msgTitle
    call PrintString

GetInputs:
    ; Input offset
    lea dx, msgOff
    call PrintString
    lea dx, bufOff
    call ReadLine0Ah

    lea si, bufOff
    call ParseHexWordFromBuf   ; result in AX, CF=error
    jc BadHex
    mov startOff, ax

    ; Input length
    lea dx, msgLen
    call PrintString
    lea dx, bufLen
    call ReadLine0Ah

    lea si, bufLen
    call ParseHexWordFromBuf   ; AX result
    jc BadHex
    mov lengthB, ax

    ; Validate length: 0001..00FF
    cmp ax, 0001h
    jb BadLen
    cmp ax, 00FFh
    ja BadLen

    ; Input mode
    lea dx, msgMode
    call PrintString
    lea dx, bufMode
    call ReadLine0Ah

    mov al, [bufMode+2]
    call ToUpperAL
    cmp al, 'H'
    je ModeOK
    cmp al, 'B'
    je ModeOK
    cmp al, 'A'
    je ModeOK
    jmp BadMode

ModeOK:
    mov mode, al

    ; Output header
    lea dx, msgOutHdr
    call PrintString

    ; Dump memory from DS:[startOff] length=lengthB
    mov si, startOff
    mov cx, lengthB
    call DumpMemory

    ; Exit prompt
    lea dx, msgPressKey
    call PrintString
    call ReadKey

    mov ax, 4C00h
    int 21h

BadHex:
    lea dx, msgErrHex
    call PrintString
    jmp GetInputs

BadLen:
    lea dx, msgErrLen
    call PrintString
    jmp GetInputs

BadMode:
    lea dx, msgErrMode
    call PrintString
    jmp GetInputs
main endp

; ------------------- DumpMemory -------------------
; IN: SI = start offset in DS, CX = length bytes
; Prints line header every 16 bytes: offset:
; Prints HEX and/or BIN depending on mode.
DumpMemory proc
    push ax
    push bx
    push dx
    push di

    xor bx, bx              ; bx = byte index for line formatting

NextByte:
    cmp cx, 0
    je DoneDump

    ; new line + offset every 16 bytes
    test bl, 0Fh
    jnz NoLineStart

    call NewLine
    mov ax, si
    call PrintHexWord
    lea dx, colonsp
    call PrintString

NoLineStart:
    mov al, [si]            ; read byte at DS:[SI]

    mov dl, mode
    cmp dl, 'H'
    je PrintHexOnly
    cmp dl, 'B'
    je PrintBinOnly
    ; else 'A' -> both
PrintAll:
    push ax                 ; FIX: preserve original AL
    call PrintHexByte
    call PrintSpace
    pop ax                  ; restore original AL
    call PrintBinByte
    call PrintSpace
    jmp Printed

PrintHexOnly:
    call PrintHexByte
    call PrintSpace
    jmp Printed

PrintBinOnly:
    call PrintBinByte
    call PrintSpace
    jmp Printed

Printed:
    inc si
    inc bx
    dec cx
    jmp NextByte

DoneDump:
    call NewLine

    pop di
    pop dx
    pop bx
    pop ax
    ret
DumpMemory endp

; ------------------- ParseHexWordFromBuf -------------------
; Parses hex digits from DOS 0Ah buffer at DS:SI.
; Buffer format: [max][len][data...]
; OUT: AX = value, CF=1 on error (non-hex or empty)
ParseHexWordFromBuf proc
    push bx
    push cx
    push dx
    push si

    xor ax, ax

    mov cl, [si+1]          ; length
    cmp cl, 0
    je ParseErr

    lea si, [si+2]          ; chars

ParseLoop:
    mov dl, [si]
    call HexCharToNibble    ; nibble in DL, CF=error
    jc ParseErr

    ; AX = AX*16 + DL
    shl ax, 1
    shl ax, 1
    shl ax, 1
    shl ax, 1
    xor dh, dh
    add ax, dx

    inc si
    dec cl
    jnz ParseLoop

    clc
    jmp ParseDone

ParseErr:
    stc

ParseDone:
    pop si
    pop dx
    pop cx
    pop bx
    ret
ParseHexWordFromBuf endp

; ------------------- HexCharToNibble -------------------
; IN: DL = ASCII char ('0'-'9','A'-'F','a'-'f')
; OUT: DL = 0..15, CF=1 if invalid
HexCharToNibble proc
    push ax

    mov al, dl
    call ToUpperAL
    mov dl, al

    cmp dl, '0'
    jb Bad
    cmp dl, '9'
    jbe IsDigit

    cmp dl, 'A'
    jb Bad
    cmp dl, 'F'
    jbe IsAlpha

Bad:
    stc
    pop ax
    ret

IsDigit:
    sub dl, '0'
    clc
    pop ax
    ret

IsAlpha:
    sub dl, 'A'
    add dl, 10
    clc
    pop ax
    ret
HexCharToNibble endp

; ------------------- PrintHexWord -------------------
; IN: AX word
; Prints 4 hex digits.
PrintHexWord proc
    push ax
    push bx
    push dx

    mov bx, ax
    mov al, bh
    call PrintHexByte
    mov al, bl
    call PrintHexByte

    pop dx
    pop bx
    pop ax
    ret
PrintHexWord endp

; ------------------- PrintHexByte -------------------
; IN: AL byte
; Prints 2 hex chars. (This version does NOT destroy AL)
PrintHexByte proc
    push ax
    push bx
    push dx

    mov bl, al              ; keep original byte

    ; high nibble
    mov al, bl
    shr al, 4
    call NibbleToHexChar
    mov dl, al
    call PrintChar

    ; low nibble
    mov al, bl
    and al, 0Fh
    call NibbleToHexChar
    mov dl, al
    call PrintChar

    pop dx
    pop bx
    pop ax
    ret
PrintHexByte endp

; ------------------- NibbleToHexChar -------------------
; IN: AL=0..15
; OUT: AL='0'..'9','A'..'F'
NibbleToHexChar proc
    cmp al, 9
    jbe Dig
    add al, 7              ; 'A' - ':' = 7
Dig:
    add al, '0'
    ret
NibbleToHexChar endp

; ------------------- PrintBinByte -------------------
; IN: AL byte
; Prints 8 bits like 01001000
PrintBinByte proc
    push ax
    push bx
    push cx
    push dx

    mov bl, al
    mov cx, 8

BinLoop:
    mov dl, '0'
    test bl, 80h
    jz BitZero
    mov dl, '1'
BitZero:
    call PrintChar
    shl bl, 1
    loop BinLoop

    pop dx
    pop cx
    pop bx
    pop ax
    ret
PrintBinByte endp

; ------------------- PrintString -------------------
; IN: DS:DX -> '$' terminated
PrintString proc
    mov ah, 09h
    int 21h
    ret
PrintString endp

; ------------------- PrintChar -------------------
; IN: DL = char
PrintChar proc
    mov ah, 02h
    int 21h
    ret
PrintChar endp

PrintSpace proc
    lea dx, space
    call PrintString
    ret
PrintSpace endp

NewLine proc
    lea dx, nl
    call PrintString
    ret
NewLine endp

; ------------------- ReadLine0Ah -------------------
; IN: DS:DX -> buffer for DOS function 0Ah
ReadLine0Ah proc
    mov ah, 0Ah
    int 21h
    ret
ReadLine0Ah endp

; ------------------- ReadKey -------------------
ReadKey proc
    mov ah, 01h
    int 21h
    ret
ReadKey endp

; ------------------- ToUpperAL -------------------
; IN/OUT: AL
ToUpperAL proc
    cmp al, 'a'
    jb NoUp
    cmp al, 'z'
    ja NoUp
    sub al, 32
NoUp:
    ret
ToUpperAL endp

end main


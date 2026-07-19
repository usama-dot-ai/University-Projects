.MODEL SMALL
.STACK 100h
.DATA

     num  DB 40

.CODE
MAIN PROC
     ; Init data segment
          MOV AX, @DATA
          MOV DS, AX

     ; Load num to AL
          MOV AL, num       ; Load num to AL
     ; Double via shift
          SHL AL, 1         ; Shift left to double

     ; Extract digits via div by 10
          MOV AH, 0         ; Clear AH for div
          MOV BL, 10        ; Divisor = 10
          DIV BL            ; AL / 10: AL=tens, AH=units

     ; Save units in BH
          MOV BH, AH

     ; Print tens
          ADD AL, 30h       ; To ASCII
          MOV DL, AL
          MOV AH, 02h
          INT 21h

     ; Print units
          MOV AL, BH        ; Load units
          ADD AL, 30h       ; To ASCII
          MOV DL, AL
          MOV AH, 02h
          INT 21h

     ; Exit
          MOV AH, 4Ch
          INT 21h
MAIN ENDP
END MAIN
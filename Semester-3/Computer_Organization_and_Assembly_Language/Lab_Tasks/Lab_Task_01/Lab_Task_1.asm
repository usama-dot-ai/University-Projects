.MODEL SMALL
.STACK 64
.DATA
    SUM      DW ?
    DIFF     DW ?
    MSG_SUM  DB 'Sum: $'
    MSG_DIFF DB 'Difference: $'
.CODE
PRINT_NUM PROC
              PUSH AX
              PUSH BX
              PUSH CX
              PUSH DX
              MOV  CX, 0
              MOV  BX, 10
    LOOP1:    
              MOV  DX, 0
              DIV  BX
              PUSH DX
              INC  CX
              CMP  AX, 0
              JNE  LOOP1
    LOOP2:    
              POP  DX
              ADD  DL, 30H
              MOV  AH, 02H
              INT  21H
              LOOP LOOP2
              POP  DX
              POP  CX
              POP  BX
              POP  AX
              RET
PRINT_NUM ENDP
MAIN PROC FAR
              MOV  AX, @DATA
              MOV  DS, AX
              MOV  AX, 0005h
              MOV  BX, 0003h
              ADD  AX, BX
              MOV  SUM, AX
              MOV  AX, 0005h
              SUB  AX, BX
              MOV  DIFF, AX
              MOV  AH, 09H
              LEA  DX, MSG_SUM
              INT  21H
              MOV  AX, SUM
              CALL PRINT_NUM
              MOV  DL, 0DH
              MOV  AH, 02H
              INT  21H
              MOV  DL, 0AH
              INT  21H
              MOV  AH, 09H
              LEA  DX, MSG_DIFF
              INT  21H
              MOV  AX, DIFF
              CALL PRINT_NUM
              MOV  DL, 0DH
              MOV  AH, 02H
              INT  21H
              MOV  DL, 0AH
              INT  21H
              MOV  AH, 4CH
              INT  21H
MAIN ENDP
END MAIN

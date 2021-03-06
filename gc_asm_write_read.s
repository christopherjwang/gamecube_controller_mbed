    AREA asm_func, CODE, READONLY
    EXPORT gc_asm_write
gc_asm_write
    PUSH    {R4-R11, LR}
    LDR     R10, =0x2009C020 ; 0x2009C020 = GPIO port 1 base address TEMPORARY
    MOV.W   R11, #0x40000000   ; 0x040000 = 1<<18 all "0"s with a "1" in bit 18 TEMPORARY
    SUB     R7, R7, R7
    
WRITE_LOOP
    
; Load GPIO Port 1 base address in register R1 
    LDR     R6, =0x2009C000 ; 0x2009C020 = GPIO port 1 base address
; Move bit mask in register R4 for bit 18 only
    MOV.W   R4, #0x00000001   ; 0x040000 = 1<<18 all "0"s with a "1" in bit 18

    MOV.W   R8, #0x00000001     ; Mask for p10 used for read

    LDR     R5, [R0, R7]
    
    CMP     R5, #0          ; value == 0 ?
    BNE     SEND_HIGH
    STR     R4, [R6,#0x1c]  ; set low
    BL      WAIT_ONE_US_MINUS_OVERHEAD
    BL      WAIT_ONE_US
    BL      WAIT_ONE_US
    B       _END_SEND_HIGH
SEND_HIGH
    STR     R4, [R6,#0x1c]  ; set low
    BL      WAIT_ONE_US
    STR     R4, [R6,#0x18]  ; set high
    BL      WAIT_ONE_US_MINUS_OVERHEAD
    BL      WAIT_ONE_US
    
_END_SEND_HIGH
    STR     R4, [R6,#0x18]
    BL       WAIT_ONE_US
    
    SUB     R1, R1, #0x01 ; going through the array backwards (it was reversed in the c layer)
    ADD     R7, R7, #0x04 ; increment address that we access using R7
    
    CMP     R1, #0
    BLT     FINISH_WRITE
    B       WRITE_LOOP
    
FINISH_WRITE
    ; send one high bit to terminate
    STR     R4, [R6,#0x1C]  ; set low
    BL       WAIT_ONE_US
    STR     R4, [R6,#0x18]  ; set high
    BL       WAIT_ONE_US_MINUS_OVERHEAD
    ;end of send one high bit to terminate
    
FINISH   
    POP     {R4-R11, LR}
    BX      LR
    
WAIT_ONE_US_MINUS_OVERHEAD
    LDR     R5, =0x8
_WAIT_ONE_US_MINUS_OVERHEAD
    SUB     R5, R5, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R5, #0
    BNE     _WAIT_ONE_US_MINUS_OVERHEAD
    BX      LR
    
WAIT_ONE_US
    LDR     R5, =0x9
_WAIT_ONE_US
    SUB     R5, R5, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R5, #0
    BNE     _WAIT_ONE_US
    BX      LR
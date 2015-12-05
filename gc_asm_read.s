    AREA asm_func, CODE, READONLY
    EXPORT gc_asm_read
gc_asm_read
    PUSH    {R4, R5, R6, R7, LR}
    LDR     R4, =0x2009C000     ; 0x2009C020 = GPIO port 1 base address
    MOV.W   R3, 0x00000002     ; Mask for p10
    SUB     R7, R7, R7          ; offset to write into the array given
    
READ_LOOP
    SUB     R1, R1, #1  ;R1 is the number of bits to read
    CMP     R1, #0
    BLT     FINISH
    
    LDR     R2, =0x999999
WAIT_FOR_HIGH
    SUB     R2, R2, #1          ; only check 0x100 times, 0x100 is arbitrarily chosen right now
    CMP     R2, #0
    BEQ     FINISH              ; TODO: add in fail case here!
    
    LDR     R5, [R4, #0x14]     ; load pin states
    AND     R5, R5, R3          ; get only p10
    CMP     R5, #0
    BEQ     WAIT_FOR_HIGH
    
    ; wait for 2us to get the bit we want
    BL      WAIT_TWO_US_MINUS_OVERHEAD
    
    LDR     R5, [R4, #0x14]     ; load pin states
    AND     R5, R5, R3          ; get only p10
    CMP     R5, #0
    BEQ     STORE_ZERO
    LDR     R5, =0x01
    STR     R5, [R0, R7]
    B       END_STORE_ZERO
STORE_ZERO
    LDR     R5, =0x00
    STR     R5, [R0, R6]
END_STORE_ZERO
    ADD     R7, R7, #0x04       ; increment address stored
    
    B READ_LOOP
    
FINISH
    POP     {R4, R5, R6, R7, LR}
    BX      LR
    
    
WAIT_TWO_US_MINUS_OVERHEAD
    LDR     R6, =0x17
_WAIT_TWO_US_MINUS_OVERHEAD
    SUB     R6, R6, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R6, #0
    BNE     _WAIT_TWO_US_MINUS_OVERHEAD
    BX      LR
    
    
    END

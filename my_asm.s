    AREA asm_func, CODE, READONLY
    EXPORT my_asm
my_asm
    PUSH    {R4, R5, R6, LR}
    SUB     R5, R5, R5
    
LOOP
    
; Load GPIO Port 1 base address in register R1 
    LDR     R4, =0x2009C000 ; 0x2009C020 = GPIO port 1 base address
; Move bit mask in register R2 for bit 18 only
    MOV.W   R2, #0x00000001   ; 0x040000 = 1<<18 all "0"s with a "1" in bit 18

    LDR     R3, [R0, R5]
    
    CMP     R3, #0          ; value == 0 ?
    BNE     SEND_HIGH
    STR     R2, [R4,#0x1C]  ; set low
    BL       WAIT_ONE_US_MINUS_OVERHEAD
    BL       WAIT_ONE_US
    BL       WAIT_ONE_US
      ; if==1, set LED1 bit ;cw: "send low"
    B       _END_SEND_HIGH
SEND_HIGH
    STR     R2, [R4,#0x1C]  ; set low
    BL       WAIT_ONE_US
    STR     R2, [R4,#0x18]  ; set high
    BL       WAIT_ONE_US_MINUS_OVERHEAD
    BL       WAIT_ONE_US
    
_END_SEND_HIGH
    STR     R2, [R4,#0x18]
    BL       WAIT_ONE_US


BACK
    
    SUB     R1, R1, #0x01 ; going through the array backwards (it was reversed in the c layer)
    ADD     R5, R5, #0x04 ; increment address that we access using R5
    
    CMP     R1, #0
    BEQ     FINISH
    B       LOOP
    
FINISH
    POP     {R4, R5, R6, LR}
    BX      LR
    
WAIT_ONE_US_MINUS_OVERHEAD
    LDR     R6, =0x8
_WAIT_ONE_US_MINUS_OVERHEAD
    SUB     R6, R6, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R6, #0
    BNE     _WAIT_ONE_US_MINUS_OVERHEAD
    BX      LR
    
WAIT_ONE_US
    LDR     R6, =0x9
_WAIT_ONE_US
    SUB     R6, R6, #1
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP     R6, #0
    BNE     _WAIT_ONE_US
    BX      LR

    

    
    
    
    
    END

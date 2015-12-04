/*
    AREA asm_func, CODE, READONLY
    EXPORT my_asm
my_asm
    PUSH    {R4, R5}
//; Load GPIO Port 1 base address in register R1 
    LDR     R5, =0x2009C000 ; 0x2009C020 = GPIO port 1 base address
; Move bit mask in register R2 for bit 18 only
    MOV.W   R2, #0x00000001   ;
    
//    MOV.W   R3, #0x00000002   ;
//    LDR     R0, [R1, #0x14];
//    AND     R0, R0, R3      ;
//    LDR R0, =0x1;
    
LOOP    
    SUB     R1, R1, #0x01
    LDR     R3, [R0, R1]
    LDR     R3, [R3, #0x01]
    
; value passed from C compiler code is in R0 - compare to a "0" 
    CMP     R3, #0          ; value == 0 ?
; (If-Then-Else) on next two instructions using equal cond from the zero flag
    ITE EQ
; STORE if EQ - clear led 1 port bit using GPIO FIOCLR register and mask
    STREQ   R2, [R5,#0x1C]
; STORE if NE - set led 1 port bit using GPIO FIOSET register and mask
    STRNE   R2, [R5,#0x18]

    
    CMP     R1, #0
    BLT     FINISH
    B      LOOP;
    
FINISH
    POP     {R4, R5}
    BX      LR
    END
*/
 
    AREA asm_func, CODE, READONLY
    EXPORT my_asm
my_asm
    PUSH    {R4, R5, R6}
    SUB     R5, R5, R5
    
LOOP
    
; Load GPIO Port 1 base address in register R1 
    LDR     R4, =0x2009C000 ; 0x2009C020 = GPIO port 1 base address
; Move bit mask in register R2 for bit 18 only
    MOV.W   R2, #0x00000001   ; 0x040000 = 1<<18 all "0"s with a "1" in bit 18

    LDR     R3, [R0, R5]
    
    CMP     R3, #0          ; value == 0 ?
    ITE EQ
    STREQ   R2, [R4,#0x1C]  ; if==0, clear LED1 bit
    STRNE   R2, [R4,#0x18]  ; if==1, set LED1 bit
    
    SUB     R1, R1, #0x01
    ADD     R5, R5, #0x04
    
    CMP     R1, #0
    BEQ     FINISH
    B       LOOP
    
FINISH
    POP     {R4, R5, R6}
    BX      LR
    END

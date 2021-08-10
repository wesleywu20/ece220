; This program evaluates postfix expressions using a stack
; It takes inputs from 0-9 and the operations +, -, /, *,and ^
; To evaluate the expression, it continually reads the input stream from the keyboard until an '=' is detected:
;   If the read value is an operand, the program pushes the read value onto the stack.
;   If the read value is an operator, the program pops two values from the stack,
;   applies the operator on the two values and pushes the result back onto the stack
;   If there is a stack underflow during any point in this process
;   or the last value before the '=' was an operand,
;   then the entered postfix expression was invalid,
;   and the program will print 'Invalid Expression' to let the user know and HALT itself.

.ORIG x3000
	
BRnzp EVALUATE ; unconditionally branch to the main loop


;R3 - value to print in hexadecimal
PRINT_HEX
    ; Register table
    ; R0 holds the current digit being read
    ; R1 holds the count of the number of digits printed for the current stack value
    ; R2 holds the count of the number of bits in the current digit being read
    ; R3 holds the value to print in hexadecimal
    
    ; callee-save registers
    ST R0, SaveR0
    ST R1, SaveR1
    ST R2, SaveR2
    ST R3, SaveR3
    ST R7, SaveR7

    ; initialize/clear registers
    AND R0, R0, #0
    AND R1, R1, #0
    ADD R1, R1, #4 ; initialize digit counter to 4
    AND R2, R2, #0
    ADD R2, R2, #4 ; initialize bit counter to 4

    ; check if 4 digits have been printed
    DIGIT_LOOP
        ADD R1, R1, #0
        BRz DONE_PRINTING

    ; if not, initialize the digit and bit counter
        AND R0, R0, #0
        AND R2, R2, #0
        ADD R2, R2, #4 ; initialize bit counter to 4

    ; check if 4 bits have been obtained from R3
    BIT_CHECK
        ADD R2, R2, #0
        BRnz CHECK_DIGIT ; if so, check number of digits printed

        ; if not, shift the digit left
        ADD R0, R0, R0
        ADD R3, R3, #0
        BRn ADD_ONE ; if MSB of R3 == 1 -> add one to the digit register

        BRnzp LSHIFT_R3 ; if not, left shift R3

    ; if MSB of R3 == 1 -> add one to the digit register
    ADD_ONE
        ADD R0, R0, #1

    LSHIFT_R3
        ADD R3, R3, R3 ; left shift the value in R3
        ADD R2, R2, #-1 ; decrement bit counter
        BRnzp BIT_CHECK ; check number of bits printed

    CHECK_DIGIT
        ADD R0, R0, #-10 ; check if the digit is <= 9
        BRn ADD_ZERO ; if so, add ASCII for '0' + 10 to digit register

    ; if digit is > 9, add ASCII value for 'A' - 10 + 10 (to "undo" the subtraction of 10 for checking)
        ADD R0, R0, #15
        ADD R0, R0, #15
        ADD R0, R0, #15
        ADD R0, R0, #15
        ADD R0, R0, #5 ; ADD ASCII for 'A' - 10 + 10 -> ADD ASCII for 'A' (65 in decimal)
        OUT ; print the digit in R0
        ADD R1, R1, #-1 ; decrement digit counter
        BRnzp DIGIT_LOOP ; check number of digits printed

    ADD_ZERO ; if digit - 10 < 0 (i.e. if digit <= 9), add ASCII for '0' + 10 (to "undo" the subtraction of 10 for checking)
        ADD R0, R0, #15
        ADD R0, R0, #15
        ADD R0, R0, #15
        ADD R0, R0, #3
        ADD R0, R0, #10 ; ADD ASCII for '0' + 10 (58 in decimal)
        OUT ; print the digit in R0
        ADD R1, R1, #-1 ; decrement digit counter
        BRnzp DIGIT_LOOP ; check number of digits printed

    ; restore registers
    DONE_PRINTING   
        LD R0, SaveR0
        LD R1, SaveR1
        LD R2, SaveR2
        LD R3, SaveR3
        LD R7, SaveR7

    RET

;R0 - character input from keyboard
EVALUATE

    GETC ; read character input from keyboard
    OUT ; echo character input from keyboard

    ; callee-save registers
    ST R1, SaveR1
    ST R2, SaveR2
    ST R3, SaveR3
    ST R4, SaveR4
    ST R6, SaveR6
    ST R7, SaveR7

    ; check if input value == '=' by comparing with inverse of ASCII for '='
    CHECK_EQUALS
        LD R1, NEG_EQUALS
        ADD R1, R1, R0
        BRz ONE_VALUE ; if so, check if one value remains on the stack

    ; check if the value is a space by comparing the input with the inverse of the ASCII for ' '
    CHECK_SPACE
        LD R1, NEG_SPACE
        ADD R1, R1, R0
        BRz RESTORE_SPACE ; if so, restore registers

    ; if input != ' ', check if it is valid
    LD R3, NEG_ZERO
    AND R4, R4, #0
    ADD R4, R0, R3 ; check if the input value is < ASCII for '0'
    BRn CHECK_OP ; if so, check if it is an operator 
                 ; (+, -, /, *; all of which have ASCII values < '0')
    
    LD R3, NEG_NINE
    AND R4, R4, #0
    ADD R4, R0, R3 ; check if input value is > ASCII for '9'
    BRp CHECK_OP ; if so, check if it is an operator
                 ; (^, the only operator with ASCII value > '9' that isn't '=',
                 ; which has already been checked for at this point)

    LD R3, NEG_ZERO
    ADD R0, R0, R3 ; subtract ASCII for '0' to push numerical value onto stack
    JSR PUSH ; if '0' <= value <= '9', then it is an operand -> push it onto the stack
    BRnzp EVALUATE

    ; check if value is an operator (+, -, /, *, ^)
    CHECK_OP
        LD R3, NEG_PLUS
        AND R4, R4, #0
        ADD R4, R0, R3 ; check if input == '+'
        BRz PERF_PLUS ; if so, perform addition

        LD R3, NEG_MIN
        AND R4, R4, #0
        ADD R4, R0, R3 ; check if input == '-' 
        BRz PERF_MIN ; if so, perform subtraction

        LD R3, NEG_MULT
        AND R4, R4, #0
        ADD R4, R0, R3 ; check if input == '*'
        BRz PERF_MULT ; if so, perform multiplication

        LD R3, NEG_DIV
        AND R4, R4, #0
        ADD R4, R0, R3 ; check if input == '/'
        BRz PERF_DIV ; if so, perform division

        LD R3, NEG_EXP
        AND R4, R4, #0
        ADD R4, R0, R3 ; check if input == '^'
        BRz PERF_EXP ; if so, perform exponentiation

        BRnzp INVALID

    PERF_PLUS
        JSR PREP ; store operands into R3 and R4
        JSR PLUS ; perform addition
        JSR PUSH ; push result of operation onto stack
        BRnzp EVALUATE
    
    PERF_MIN
        JSR PREP ; store operands into R3 and R4
        NOT R3, R3
        ADD R3, R3, #1 ; invert second operand
        JSR MIN ; perform subtraction
        JSR PUSH ; push result of operation onto stack
        BRnzp EVALUATE
    
    PERF_MULT
        JSR PREP ; store operands into R3 and R4
        JSR MUL ; perform multiplication
        JSR PUSH ; push result of operation onto stack
        BRnzp EVALUATE
    
    PERF_DIV
        JSR PREP ; store operands into R3 and R4
        JSR DIV ; perform division
        JSR PUSH ; push result of operation onto stack
        BRnzp EVALUATE
    
    PERF_EXP
        JSR PREP ; store operands into R3 and R4
        JSR EXP ; perform exponentiation
        JSR PUSH ; push result of operation onto stack
        BRnzp EVALUATE

    ; if input == '=', check if stack only has one value
    ONE_VALUE
        LD R3, STACK_TOP
        NOT R3, R3
        ADD R3, R3, #1 ; invert R3
        LD R4, STACK_START
        ADD R4, R4, #-1
        ADD R4, R4, R3 ; check if (R4 - 1) - R3 == 0 
                       ; (i.e. if STACK_TOP == STACK_START - 1)
        BRp INVALID ; if not, stack has > 1 value and
                    ; user has entered an invalid expression

    ; if so, load result in R5
    JSR POP
    AND R3, R3, #0
    ADD R3, R0, R3
    ADD R5, R5, R0 ; store result of operation(s) into R3 (used by PRINT_HEX) and R5
    JSR PRINT_HEX
    BRnzp DONE

    ; if there is an invalid expression for any reason, print the message,
    ; restore the registers, then halt the program
    INVALID
        LEA R0, INVALID_EXP
        PUTS ; print "Invalid Expression"
        ; callee-restore registers
        LD R1, SaveR1
        LD R2, SaveR2
        LD R3, SaveR3
        LD R4, SaveR4
        LD R6, SaveR6
        LD R7, SaveR7
        BRnzp DONE ; halt program

    ; if entered character is a space, 
    ; restore the registers to their original values
    RESTORE_SPACE
        ; callee-restore registers
        LD R1, SaveR1
        LD R2, SaveR2
        LD R3, SaveR3
        LD R4, SaveR4
        LD R6, SaveR6
        LD R7, SaveR7
        BRnzp EVALUATE ; branch back to beginning of loop

    AND R5, R5, #0
    ADD R5, R5, R0 ; store result of operation(s) into R5

    ; callee-restore registers
    LD R1, SaveR1
    LD R2, SaveR2
    LD R3, SaveR3
    LD R4, SaveR4
    LD R6, SaveR6
    LD R7, SaveR7
    JSR PRINT_HEX
    BRnzp DONE ; halt program

SaveR0      .BLKW #1
SaveR1      .BLKW #1
SaveR2      .BLKW #1
SaveR3      .BLKW #1
SaveR4      .BLKW #1
SaveR5      .BLKW #1
SaveR6      .BLKW #1
SaveR7      .BLKW #1

;input R3, R4
;out R0 (R3+R4)
PLUS	
;your code goes here
    ADD R0, R3, R4
    RET

;input R3 (already inverted), R4
;out R0 (R4-R3)
MIN
    ADD R0, R3, R4
    RET	
	
;input R3, R4
;out R0 (R3*R4)
MUL	
    AND R0, R0, #0 ; clear R0
    ; callee-save registers
    ST R4, SaveR4
    ST R7, SaveR7

    MULT_LOOP
        ADD R0, R0, R3 ; add value in R3 to R0 R4 (number of) times
        ADD R4, R4, #-1 ; decrement multiplication loop counter
        BRp MULT_LOOP
    
    LD R4, SaveR4
    LD R7, SaveR7
    RET
	
;input R3, R4
;out R0 (R4/R3)
DIV
    ; callee-save registers
    ST R1, SaveR1
    ST R2, SaveR2
    ST R4, SaveR4
    ST R7, SaveR7

    NOT R2, R3
    ADD R2, R2, #1 ; invert R3 (divisor), save into R2
    AND R0, R0, #0
    ADD R0, R0, #-1 ; initialize R0 (quotient) to be -1
    DIV_LOOP
        ADD R0, R0, #1 ; add one to quotient for each subtraction
        ADD R4, R4, R2 ; keep subtracting divisor from dividend until dividend < 0
        BRzp DIV_LOOP
    
    ; callee-restore registers
    ST R1, SaveR1
    ST R2, SaveR2
    ST R4, SaveR4
    ST R7, SaveR7
    RET

;input R3, R4
;out R0 (R4^R3)
EXP
    ; callee-save registers
    ST R2, SaveR2
    ST R3, SaveR3
    ST R5, SaveR5
    ST R7, SaveR7

    AND R0, R0, #0
    AND R2, R2, #0
    ADD R2, R2, R3
    ADD R2, R2, #-1 ; initialize R2 to be exponent value - 1
    AND R3, R3, #0
    ADD R3, R3, R4 ; initialize R3 to be base value
    AND R5, R5, #0
    ADD R5, R5, R4 ; initialize R5 to be base value
    EXP_LOOP
        AND R0, R0, #0 ; clear R0 before performing multiplication
        MULT_EXP_LOOP
            ADD R0, R0, R4 ; add value being multiplied to R0
            ADD R3, R3, #-1 ; decrement multiplication counter
            BRp MULT_EXP_LOOP
        AND R3, R3, #0
        ADD R3, R3, R5 ; reset R3 to base value
        AND R4, R4, #0
        ADD R4, R4, R0 ; update value to be multiplied
        ADD R2, R2, #-1 ; decrement exponent value counter
        BRp EXP_LOOP
    
    ; callee-restore registers
    LD R2, SaveR2
    LD R3, SaveR3
    ST R5, SaveR5
    LD R7, SaveR7
    RET

; attempts to pop two operands off of the stack
; to prepare to perform an operation
; output 
PREP
    ST R7, SaveR7 ; callee-save R7

    JSR POP
    ADD R5, R5, #0 
    BRp INVALID ; if first pop results in underflow, print "Invalid Expression"
    AND R3, R3, #0
    ADD R3, R3, R0 ; if not, store the popped value into R3
    JSR POP
    ADD R5, R5, #0
    BRp INVALID  ; if second pop results in underflow, print "Invalid Expression"
    AND R4, R4, #0 ; if not, store the popped value into R4
    ADD R4, R4, R0

    LD R7, SaveR7 ; callee-restore R7
    RET

;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACK_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET

PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;

;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET

DONE
    HALT

POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

NEG_EQUALS  .FILL xFFC3 ; 2's complement additive inverse of '='
NEG_SPACE   .FILL xFFE0 ; 2's complement additive inverse of ' '
NEG_ZERO    .FILL xFFD0 ; 2's complement additive inverse of '0'
NEG_NINE    .FILL xFFC7 ; 2's complement additive inverse of '9'
NEG_PLUS    .FILL xFFD5 ; 2's complement additive inverse of '+'
NEG_MIN     .FILL xFFD3 ; 2's complement additive inverse of '-'
NEG_MULT    .FILL xFFD6 ; 2's complement additive inverse of '*'
NEG_DIV     .FILL xFFD1 ; 2's complement additive inverse of '/'
NEG_EXP     .FILL xFFA2 ; 2's complement additive inverse of '^'

INVALID_EXP .STRINGZ "Invalid Expression"

.END
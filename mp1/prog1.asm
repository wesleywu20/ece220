;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming video lecture, we will discuss 
;  how to prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** Submit a working version to Gradescope  **



	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST

; you will need to insert your code to print the histogram here

; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments

; this approach uses an outer loop to count how many digits
; in a line have been printed, and prints a new line after
; 4 digits have been printed
; with each new line, the current character (bin) of the histogram
; that is being printed is printed out along with a space
; before entering the inner loop, which
; counts how many bits of one digit of the current bin have been recorded
; once the inner loop reads 4 digits, a new line is printed,
; and the outer loop moves into the next iterations
; the outer loop iterates 27 times,
; one iteration per bin in the histogram

; Register table
; R0 holds the current digit being read
; R1 holds the count of the number of digits printed for the current histogram entry
; R2 holds the count of the number of bits in the current digit being read
; R3 holds the current histogram entry from which the digits are being read
; R4 holds the count of the number of bins of the histogram to be printed (loop counter)
; R5 holds the memory address of the beginning of the histogram (a constant)
; R6 holds the number of bins of the histogram that have been printed

; initialize/clear registers
AND R0, R0, #0
AND R1, R1, #0
ADD R1, R1, #4 ; initialize digit counter to 4
AND R2, R2, #0
ADD R2, R2, #4 ; initialize bit counter to 4
AND R3, R3, #0
AND R4, R4, #0
LD R4, NUM_BINS ; initialize loop counter to 27
AND R5, R5, #0
LD R5, HIST_ADDR ; initialize R5 to be the starting address of the histogram
AND R6, R6, #0

PRINT_CURR_CHAR ; print the current character (bin) of the histogram
LD R0, FIRST_CHAR
ADD R0, R0, R6 ; store the ASCII value of the first character + number of bins already printed in R0
OUT ; print current character
LD R0, SPACE
OUT ; print a space
AND R0, R0, #0 ; reset digit register
ADD R3, R5, R6 ; store address of current bin of histogram being read into R3
LDR R3, R3, #0 ; store the histogram entry of the memory address stored in R3 into R3

; check if 4 digits have been printed
DIGIT_LOOP
ADD R1, R1, #0
BRz NEW_LINE ; if so, print a newline character

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

; print a new line
NEW_LINE
LD R0, NEWLINE 
OUT ; print a newline character
AND R1, R1, #0
ADD R1, R1, #4 ; reset digit counter to 4
ADD R6, R6, #1 ; increment character counter
ADD R4, R4, #-1 ; decrement bin counter
BRp PRINT_CURR_CHAR ; if number of bins to print > 0, keep printing

DONE	HALT			; done

; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00 ; histogram starting address
STR_START	.FILL x4000	; string starting address
NEWLINE 	.FILL x000A ; newline character
SPACE		.FILL x0020 ; space
FIRST_CHAR	.FILL x0040 ; @ symbol

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
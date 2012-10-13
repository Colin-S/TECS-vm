// init SP
@256
D=A
@SP
M=D

(pushtrue)
@SP
A=M
M=-1
(pushfalse)
@SP
A=M
M=0

// are 2 and 1 equal?
@SP
AM=M-1
D=M // D = 2
@SP
AM=M-1 // M = 1
D=D-A // D = 0 if they are equal
@pushtrue
D; JEQ // push true if 0
@pushfalse
0; JMP // else push false
// TODO: need to create a unique label each time EQ/LT/GT is called
//	need to pre-define these functions in the beginning, create new label,
//	push that label on the stack, jump to these functions, then return

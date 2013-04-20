// init code ///////////////////
//init stack pointer
@256
D=A
@SP
M=D

// skip past the init code to the start
@programstart
0;JMP 

(pushtrue)
@SP
A=M
M=-1
// increment stack pointer
@SP
M=M+1
// return to where we came from
A=D
0;JMP

(programstart)

// program code ///////////////////

// Compare top two values of stack
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M-D

// save the return location
@l1 // this label is autogenerated
D=A

// push true onto the stack
@pushtrue
0;JMP

(l1) //return location
D=A

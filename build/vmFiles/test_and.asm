// Init Code ////////////
// Init SP
@256
D=A
@SP
M=D

@programStart
0;JMP

(pushTrue)
@SP
A=M
M=-1
@SP
M=M+1
@R13
A=M;JMP

(pushFalse)
@SP
A=M
M=0
@SP
M=M+1
@R13
A=M;JMP

(or)
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M|D
@SP
M=M+1
@R13
A=M;JMP

(and)
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M&D
@SP
M=M+1
@R13
A=M;JMP

(programStart)

// Program Code /////////
// push constant 5
@5
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 14
@14
D=A
@SP
A=M
M=D
@SP
M=M+1
// And
@l0
D=A
@R13
M=D
@and
0;JMP
(l0)

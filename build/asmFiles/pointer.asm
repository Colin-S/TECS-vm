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

(not)
@SP
M=M-1
A=M
M=!M
@SP
M=M+1
@R13
A=M;JMP

(neg)
@SP
M=M-1
A=M
M=-M
@SP
M=M+1
@R13
A=M;JMP

(add)
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M+D
@SP
M=M+1
@R13
A=M;JMP

(sub)
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M-D
@SP
M=M+1
@R13
A=M;JMP

(push)
@R14
A=M
D=A
@SP
A=M
M=D
@SP
M=M+1
@R13
A=M;JMP

(programStart)

// Program Code /////////
// Push constant 7
@l0
D=A
@R13
M=D
@7
D=A
@R14
M=D
@push
0;JMP
(l0)

// Push constant 9
@l1
D=A
@R13
M=D
@9
D=A
@R14
M=D
@push
0;JMP
(l1)

// Pop pointer 0
@0
D=A
@THIS
D=A+D
@R14
M=D
@SP
AM=M-1
D=M
@R14
A=M
M=D

// Pop pointer 1
@1
D=A
@THIS
D=A+D
@R14
M=D
@SP
AM=M-1
D=M
@R14
A=M
M=D

// Push pointer 0
@l2
D=A
@R13
M=D
@0
D=A
@THIS
A=A+D
D=M
@R14
M=D
@push
0;JMP
(l2)

// Push pointer 1
@l3
D=A
@R13
M=D
@1
D=A
@THIS
A=A+D
D=M
@R14
M=D
@push
0;JMP
(l3)

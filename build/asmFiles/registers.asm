// Test using register to store program counter location,
//  for jumping to label, then returning

// init code ///////////////////

//init stack pointer
@256
D=A
@SP
M=D

// skip past the init code to the start
@programstart
0;JMP 

// Do stuff
(stuff)
@5
D=A
@SP
A=M
M=D
@SP
M=M+1
// return
@R13
A=M;JMP

(programstart)

// program code ///////////////////

// Store return location
@l1
D=A
@R13
M=D

// Do stuff
@stuff
0;JMP

// return location
(l1)

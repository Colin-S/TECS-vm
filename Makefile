# These are variables
# They are referenced with the $() syntax later on
# We are using the GNU gcc compiler, which is the most commonly used open-source compiler
# The following are gcc compiler options:
#	-Wall = compile with all warnings
#	-O0 = use level 0 compiler optimization
#	-sdt=c99 = compile using the C99 standard (the newest one with the best features)
OPTIONS=-Wall -O0 -std=c99 
EXECUTABLE=vm
COMPILER=gcc

# all: is the start point of the makefile (like main in C)
# The indentation of 1 tab below all: is required for this makefile to run properly
all:
	$(COMPILER) main.c -o $(EXECUTABLE) $(OPTIONS)

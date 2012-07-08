OPTIONS=-Wall -O0 -std=c99
EXECUTABLE=vm

all:
	gcc main.c -o $(EXECUTABLE) $(OPTIONS)

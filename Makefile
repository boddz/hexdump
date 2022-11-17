CC = gcc
STD = c99
BIN = hexdump

all: main

main:
	$(CC) -std=$(STD) main.c -o $(BIN)

clean:
	rm $(BIN)

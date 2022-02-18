EXEC = encode decode
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC = clang
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra -O2
LDFLAGS = -lm

all: encode decode

encode: encode.o huffman.o stack.o node.o code.o io.o pq.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

decode: decode.o huffman.o stack.o node.o code.o io.o pq.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

tidy: 
	rm -rf $(OBJECTS)

clean: tidy
	rm -rf $(EXEC) 

format:
	clang-format -i -style=file *.[ch]

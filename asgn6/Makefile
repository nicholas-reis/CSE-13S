EXEC = keygen encrypt decrypt
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC = clang
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra -O2 $(shell pkg-config --cflags gmp)
LDFLAGS = -lm $(shell pkg-config --libs gmp)

all: keygen encrypt decrypt

keygen: keygen.o numtheory.o randstate.o rsa.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

encrypt: encrypt.o numtheory.o rsa.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

decrypt: decrypt.o numtheory.o rsa.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

tidy: 
	rm -rf $(OBJECTS)

clean: tidy
	rm -rf $(EXEC) 

format:
	clang-format -i -style=file *.[ch]

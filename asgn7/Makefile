EXEC = banhammer
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC = clang
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra -O2
LDFLAGS = -lm 

all: banhammer 

banhammer: node.o bst.o speck.o ht.o bv.o bf.o parser.o banhammer.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

tidy: 
	rm -rf $(OBJECTS)

clean: tidy
	rm -rf $(EXEC) 

format:
	clang-format -i -style=file *.[ch]

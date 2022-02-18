# The Great Firewall of Santa Cruz - README.md
## Nicholas Reis
### December 5, 2021

---
## Program Description
In this assignment, I created a program that filers out words that are marked as bad or old
and either replaces them with a new word or scolds the user for thoughtcrimes. The file
that does this is banhammer.c, which uses functionality from many other files to work
properly. These files include: speck.c that hashes words, ht.c that implements a hash
table ADT, bst.c that implements binary search tree functionality, node.c that implements
a node ADT, bf.c that implements a bloom filter ADT, bv.c that implements a bit vector ADT,
and parser.c that parses words based on regular expressions.

---

## How to Build and Run the Program
Once you have all the necessary files in a repository - including speck, ht, bst, node, bf, bv, and parsers' respective
.c and .h files, the Makefile, banhammer.c, messages.h, and salts.h - the user can type "make" into the command line to 
compile the code together into a binary (banhammer) that can run the program. The Makefile also has the commands "make format",
"make tidy" and "make clean". Format will clang-format all of the files, tidy will remove all of the object files, and 
clean will remove all of the executables along with running tidy. In order to run my program, simply type "./banhammer -h"
after first typing "make". This will print out some help text with a synopsis and usage, how to use the test harness, and 
describes all of the available options. The options include "-h -s -t size -f size". -h prints out this usage manual to stderr.
-s prints out statistics of the program. -t size specifies the desired hash table size. -f size specifies the desired bloom filter
size. An example of running the program is: ./banhammer < words.txt -s -t 1000
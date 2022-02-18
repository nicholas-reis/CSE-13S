# Huffman Coding - README.md
## Nicholas Reis
### November 7, 2021

---

## Program Description
In this program, I created an encoder that takes in a file and compresses it into a smaller file,
along with a decoder the takes in a compressed file and decompresses it into the original file.
This required me to create five abstract data types - Code, IO, Node, PriorityQueue, and Stack -
along with a file huffman.c that contains implementation for functions that were used in my
encoder and decoder.

---

## How to Build and Run the Program
Once you have all the necessary files in a repository - including each ADT's individual .c and .h file,
huffman.c/.h, defines.h, the Makefile, encode.c, and decode.c - the user can type "make" into the command
line to compile the code together into two binaries (encode/decode) that can separately be run to encode
or decode a file. The Makefile also has the commands "make format", "make tidy" and "make clean". Format
will clang-format all of the files, tidy will remove all of the object files, and clean will remove all 
of the executables along with running tidy. In order to run my program, simply type "./encode -h" or 
"./decode -h" after first typing "make". This will print out some help text with a synopsis and usage,
how to use the test harnesses, and describes all of the available options. The options include 
"-h -v -i input -o output". -h prints out this usage manual to stderr. -v prints out stats of file sizes.
-i input takes in a specific input file and takes values from there directly. -o output specifies a file 
where the output should be printed. If -i and -o are not specified, the program takes an input from stdin 
and outputs to stdout.
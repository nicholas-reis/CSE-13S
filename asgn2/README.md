# A Little Slice of Pi - README.md
## Nicholas Reis
### October 9, 2021

---

## Program Description
In this program, I wrote four functions that calculate the approximate value
of pi utilizing four different formulas, along with a function that approximates
the value of Euler's number *e*, and a function that approximates the square
root of a number. 

---

## How to Build and Run the Program
Once you have all the necessary files in a repository - including each function's
individual .c file, mathlib.h, Makefile, and mathlib-test.c - the user can
type "make" into the command line to compile the code together into a binary that
can be run to show desired outputs. The Makefile also has the commands "make format",
"make tidy" and "make clean". Format will clang-format all of the files, tidy will
remove all of the object files, and clean will remove all of the executables along
with running tidy. In order to run my program, simply type "./mathlib-test.c" after
first typing "make". This will print out some text with a synopsis, how to use the
test harness, and describes all of the available options. Some of the options include
"-m -v -b -r", which will all print out my approximations of pi in comparison to
the value of pi from the math.h library. "-e" will print out my approximation of *e*
in comparison to the value of *e* from the math.h library. "-s" prints out the
number of iterations it took each function to reach the approximation. "-h" will
print the aforementioned help section. "-a" will print out all of the options.
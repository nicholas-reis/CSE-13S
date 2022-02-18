# Sorting: Putting your affairs in order - README.md
## Nicholas Reis
### October 14, 2021

---

## Program Description
In this program, I wrote four functions that sort a given array utilizing four
different sorting algorithms, along with a test harness that takes in user
options and prints out the sorted array. The four algorithms are heap sort,
insertion sort, shell sort, and quick sort, which all sort the array correctly
with differing methods and efficiency.

---

## How to Build and Run the Program
Once you have all the necessary files in a repository - including each function's
individual .c and .h file, stats.h and stats.c, set.h, Makefile, and sorting.c - 
the user can type "make" into the command line to compile the code together into a 
binary (sorting) that can be run to show desired outputs. The Makefile also has the
commands "make format", "make tidy" and "make clean". Format will clang-format all 
of the files, tidy will remove all of the object files, and clean will remove all 
of the executables along with running tidy. In order to run my program, simply type 
"./sorting" after first typing "make". This will print out some help text with a synopsis,
how to use the test harness, and describes all of the available options. Some of the 
options include "-i -e -s -q", which will all print out the sorted array created by
using pseudorandom numbers and a default seed of 13371453 (i uses insertion sort,
e uses heap sort, s uses shell sort, and q uses quick sort). "-r" followed by an 
integer allows the user to manually enter a seed that will change the pseudorandomly 
generated numbers in the array. "-n" followed by an integer changes the size of the
array. "-p" followed by an integer tells the program how many numbers in the array
to be printed out. "-h" will print the aforementioned help section. "-a" will print 
out all of the options.
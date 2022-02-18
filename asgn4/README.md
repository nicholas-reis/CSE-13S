# Perambulations of Denver Long - README.md
## Nicholas Reis
### October 23, 2021

---

## Program Description
In this program, I wrote three abstract data types - Graph, Stack, and Path -
along with a test harness that reads in a file with the number of vertices
in a graph, the names of the vertices, and the edges of a matrix. The program
uses depth-first searching to find the shortest Hamiltonian path within
the matrix.

---

## How to Build and Run the Program
Once you have all the necessary files in a repository - including each ADT's
individual .c and .h file, vertices.h, the Makefile, and tsp.c - the user
can type "make" into the command line to compile the code together into a
binary (tsp) that can be run to show the findings of the depth-first
search function. The Makefile also has the commands "make format", "make tidy"
and "make clean". Format will clang-format all of the files, tidy will remove 
all of the object files, and clean will remove all of the executables along 
with running tidy. In order to run my program, simply type "./tsp" after first
typing "make". This will print out some help text with a synopsis and usage,
how to use the test harness, and describes all of the available options. The
options include "-h -v -u -i input -o output". -h prints out this usage manual
to stdout. -v prints out every Hamiltonian path as it is found in a graph. -u
makes the graph undirected, meaning each vertex that has an edge to another
vertex will also have an edge going the opposite direction. -i input takes
in a specific input file and takes values from there directly. -o output
specifies a file where the output should be printed. If -i and -o are not
specified, the program takes an input from stdin and outputs to stdout.
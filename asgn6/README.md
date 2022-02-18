# Public Key Cryptography - README.md
## Nicholas Reis
### November 18, 2021

---
## Program Description
In this program, I created a public and private key generator, a file encryptor, and a file
decryptor. The key generator - keygen.c - uses functions written in rsa.c to create a public 
key and a private key before writing them to specified output files. The encryptor - encrypt.c -
reads in information from a given public key, verifies that the signature of the key is valid,
and then encrypts the specified input file. The decryptor - decrypt.c - reads in information
from a given private key before decrypting the specified encrypted input file. The functions
I utilized in each of these files were written in other files - numtheory.c, which implemented
many of the algorithms behind the program, randstate.c, which implemented function to initialize
and clear a random state used to generate random numbers, and rsa.c, which implemented the functions
used to read and write public and private keys, along with encrypting and decrypting files.

---

## How to Build and Run the Program
Once you have all the necessary files in a repository - including rsa, numtheory, and randstates' .c and .h file,
the Makefile, keygen.c, encrypt.c, and decrypt.c - the user can type "make" into the command line to compile the 
code together into three binaries (keygen/encrypt/decrypt) that can separately be run to create public and private
keys, encrypt, or decrypt a file. The Makefile also has the commands "make format", "make tidy" and "make clean". 
Format will clang-format all of the files, tidy will remove all of the object files, and clean will remove all 
of the executables along with running tidy. In order to run my program, simply type "./keygen -h", ./encrypt -h" or 
"./decrypt -h" after first typing "make". This will print out some help text with a synopsis and usage, how to use 
the test harnesses, and describes all of the available options. The options include "-h -v -b bits -i iters -n pbfile
-d pvfile -s seed" for keygen.c. -h prints out this usage manual to stderr. -v prints out specific mpz_t values. -b bits
specifies the number of bits that the keys should use. -i iters specifies the number of iters to be used. -n pbfile
specifies the file where the public key will be written. -d pvfile specifies the file where the private key should be
written. -s seed specifies the seed for the random numbers. The options for encrypt and decrypt include "-h -v -i
infile -o outfile -n pbfile/pvfile". -h and -v are the same concept as keygen.c. -i infile specifies the input file 
that will be read from. -o outfile specifies the output file that will be written to. -n specifies either the public
key file or private key file respectively.
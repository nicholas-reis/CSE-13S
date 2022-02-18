# Pass the Pigs - README.md
## Nicholas Reis
### October 3, 2021

---

## Program Description
This program takes in two user inputs before beginning the game.
The first input is the number of players between 2 and 10, and the
second input is a seed between 0 and the max unsigned integer. The
program will then randomly simulate a dice rolling game (except it's
with pigs), which gives points to players depending on which side is
rolled. The game prints out who is rolling and what they roll up until
one player reaches a score of 100, in which the program prints out the
name of the winner and how many points they scored. Then the program
comes to an end.

---

## How to Build and Run the Program
Once you have all the necessary files in a repository (particularly
the Makefile, names.h, and pig.c) simply type "make" into the
command line and press enter. This will format the code for you
and also build the program. You can then type "./pig", and then
the program will begin. It first asks to input a player count and
then a seed, and then will simulate once they are entered.


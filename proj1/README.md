# Homework 1
All files located in proj1 zip
## Run Instructions
To run,
1. unzip proj1
2. navigate to the proj1 folder on your system
3. run "make"
4. run "./calculate <INSERT EQUATION>
5. command line arguments shoudl be separated by a space, Ex "./calculate 2 / 3 + 4"
6. available operations are /, +, x, and -

## Files
### main.cc
main.cc contains the entry point for the project. It takes in arguments through a loop and passes them to the run() method in calculator.h
### calculator.h and calculator.cc
calculator.h is the header file for calculator.cc. It contains a list of methods used in the cc file. calculator.cc includes the methods that calculate the output. This includes add, subtract, divide, multiply, multDiv, and addSub. The first four work as they are named. multDiv processes all multiplication/division statements in the equation by performing the operation, removing the floats immediately to either side of the operand in the vector, and replaces the operand with the result of the operation. addSub works in the same way and runs after multDiv to follow PEMDAS. The final vector is then printed to the console.
### makefile
the makefile contains all targets needed to run the program, as well as a "clean" feature.\
\
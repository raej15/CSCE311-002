# Homework 2
All files located in proj1 zip
## Run Instructions
To run,
1. unzip proj2
2. navigate to the proj2 folder on your system
3. run "make"
4. run "./server \<INSERT SOCKET_NAME\> in one terminal
5. run "./client \<INSERT SOCKET_NAME\> \<INSERT FILE_PATH\> \<INSERT LINE NUMBERS> in another terminal
6. command line arguments should be separated by a space, Ex "./client t.socket ./dat/equations_1.txt 2 3 6 "

## Files
### myServer.h and myServer.cc
myServer.h is the header file for myServer.cc. myServer.cc hosts client requests for calculations. It also prints the path and lines read from the client and sends the information back.
### myClient.h and myClient.cc
myClient.h is the header file for myClient.cc. myClient.cc sends the text file path and file lines to the server. It then receives the data in the lines from the server and computes the answers to the equations. 
### makefile
the makefile contains an all target, a client target, a server target, as well as a "clean" target.
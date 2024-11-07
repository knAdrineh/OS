The code is in c++

To compile follow these two steps:

STEP 1:  THE CPP FILE NAMES
g++ -o main main.cpp VA.cpp VM.cpp
STEP 2:
pass in the init, input and output file names as a argument 
you can change the init, input and  output file names as indeeded

WITH DP: 
./main init-dp.txt input-dp.txt output-dp.txt

WITHOUT DP:
./main init-no-dp.txt input-no-dp.txt output-no-dp.txt

FILE SPECIFICATIONS: 
main.cpp: takes arguments from the command line, calls the init() function from the VM and then passes the input, and output for processing commands
VM.cpp : is the virtual manager, that contains the functions:
     init(string filePath) takes the filepath and initalizes the PM[], Disk[][] and frames[]
     printPM() prints all the data Structurs PM[] Disk[][] and frames[]
     processCommands(string input_file_name, string output_file_name) read commands from a input file process them and write it to the output file
     void read_block(int b, int m)  copies the entire block D[b] into the PM frame starting at location PM[m]. 
    int nextFree() finds the next available free frame
VM.h : has the declarations of the function headers
VA.cpp/.h : a class of virtual address translation to get the s, p, w, and pw values
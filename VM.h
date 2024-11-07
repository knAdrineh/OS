#include <vector>
using namespace std;

//resets all the data structures including PM, frames, and the disk
void resetAll();

//initialize the PM
void init(string filePath);

//print the PM 
void printPM();

//read commands from a input file process them and write it to the output file
void processCommands(string input_file_name, string output_file_name);

//read_block(b, m) copies the entire block D[b] into the PM frame starting at location PM[m]. 
void read_block(int b, int m);

//find the available free frame
int nextFree();
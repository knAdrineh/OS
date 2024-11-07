#include <iostream>
#include "VA.h"
#include <vector>
#include "VM.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;




int main(int argc, char *argv[]){


    // if number of arguments is not 4 
    if(argc != 4)
        cout << "USAGE : \n  ./main init.txt input.txt output.txt" <<endl;
    else{
        resetAll();
        string init_file_name = argv[1];
        string input_file_name = argv[2];
        string output_file_name = argv[3];
        //initalize the PM
        init(init_file_name);
        cout << endl;
        cout << "After init:" << endl;
        printPM();
        // cout << endl;
        //proccess the commands in the input file and write it to the outputfile
        processCommands(input_file_name, output_file_name);
        // VA nv(2097162);

        cout << endl;
        cout << "After all operations:" << endl;
        printPM();
    }
    
    return 0;
}
#include <vector>
#include <fstream>
#include <cerrno>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include "VM.h"
#include "VA.h"

using namespace std;

// The physical memory is 524288 and it is divided into 1024 frames

const int PAGE_SIZE = 512;
const int PT_SIZE = 512;
const int ST_SIZE = 1024;

const int PM_SIZE= 524288;
vector<int> PM(PM_SIZE);
vector<int> frames(1024);
int next_available_frame = 0;

// For demand paging, use a 2D vector for the disk
vector< vector<int> > disk(1024, vector<int>(512));

// Resets all the data structures including PM, frames, and the disk
void resetAll(){
    fill(PM.begin(), PM.end(), 0);
    fill(frames.begin(), frames.end(), 0);
    for(int i = 0; i<ST_SIZE; i++){
        for(int j= 0; j<PT_SIZE; j++){
            disk[i][j] = 0;
        }
    }
    //Segment Table occupies this
    frames[0] = 1;
    frames[1] = 1;
    next_available_frame = 2;
    // cout << next_available_frame << endl;
}

// Initialize the PM
void init(string filePath){
    try {
        ifstream infile(filePath);
        if (!infile) {
            throw std::runtime_error("File could not be opened.");
        }

        // Read first line
        string line_one;
        getline(infile, line_one);
        
        // cout << line_one << endl;

        // use the sstream to split the line by tuples
        istringstream iss1(line_one);
        int segment; 
        int len;
        int frame;

         // PM[2*segment] = len and PM[2*segment + 1] = frame
        while (iss1 >> segment >> len >> frame)
        {       
                if (frames[frame] != 1 && segment < PM_SIZE/2 ){
                    PM[2*segment] = len;
                    PM[2*segment + 1] = frame;
                    if (frame > 1) {
                        frames[frame] = 1; //set the frame as used
                    }
                }
                
        }
        // cout<< "Read frist line" << endl;
        // printPM();
        // cout << endl;
        // Read second line
        string line_two;
        getline(infile, line_two);
        istringstream iss2(line_two);
        int page;

        // PM[PM[2*segment2 + 1]*512 + page] = frame2
        while (iss2 >> segment >> page >> frame)
        {
            if (frames[frame] != 1 && segment < PM_SIZE/2 ){
                if( PM[2*segment + 1] < 0 ){
                    disk[abs(PM[2*segment+1])][page] = frame;
                
                }
                else{
                    PM[PM[2*segment + 1]*512 + page] = frame;
                }
                if(frame > 0 ) 
                    frames[frame] = 1; //set frame as used
            }

        }
        // cout<< "Read second line" << endl;
        // printPM();
        // cout << endl;
        infile.close();
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

void printPM(){
    for (int i = 0; i<PM.size(); i++){
        if(PM[i] != 0)
            cout << "PM[" << i << "]="  <<  PM[i] << " ";
    }
    cout << endl;
    for(int i = 0; i<ST_SIZE; i++){
        for(int j= 0; j<PT_SIZE; j++){
             if(disk[i][j] != 0)
            cout << "Disk [" << i << "][" << j <<"]="  <<  disk[i][j] << " ";
        }
    }
    cout <<  endl;
     for (int i = 0; i<ST_SIZE; i++){
        if(frames[i] != 0)
            cout << "frames[" << i << "]="  <<  frames[i] << " ";
    }
    cout << endl << endl;

}

//read commands from a input file process them and write it to the output file
void processCommands(string input_file_name, string output_file_name){
    try
    {
        ifstream infile(input_file_name);
        ofstream outfile(output_file_name);
        string line;
        string command;
        int vaddress;

        while(getline(infile, line)){
            istringstream iss(line);
            if(iss >> command )
            {
                // cout << vaddress << endl;
                // cout << command  << " "<< vaddress << endl;
                if(command == "TA"){ //if the command is to translate the address 
                    iss >> vaddress;
                    VA va(vaddress); //create a new VA and assign the integer vaddress 
                    int pw = va.getPW(); //get the PW
                    int s = va.getS(); 
                    int p = va.getP();
                    int w = va.getW();
                    // cout << vaddress << endl;
                    // cout << "s : " << va.getS() << endl;
                    // cout << "p : " << va.getP() << endl;
                    // cout << "w : " << va.getW() << endl;
                    // cout << "pw : " << va.getPW() << endl;
                    if( s > PM_SIZE/2){
                        outfile << -1 << " ";
                        continue;
                    }
                       
                

                    if(pw >= PM[2*s]){
                        // cout << "pw = " << pw << ">= PM[2*"<< s<< "]=" << PM[2*s] << endl;
                        // cout << pw << endl;
                        // cout << s << endl;
                        // cout << PM[2*s] << endl;
                        // cout << "cout writing -1 to the file" << endl;
                        outfile << -1 << " ";
                        continue;
                
                    }
                    if(PM[2*s + 1] < 0) ///* page fault: PT is not resident */
                    {
                        // cout << "page fault: PT is not resident" << endl;
                        // cout << "PM[2*" << s <<" + 1] : " << PM[2*s + 1] << endl;
                        //Allocate free frame f1 using list of free frames
                        int f1 = nextFree();
                        // cout << "the free frame is = " << f1 << endl;
                        // cout << "f1 1 : " << f1 << endl;
                        frames[f1] = 1;
                        //now that the frame is full the next one is available
                        //Update list of free frames
                        ++next_available_frame;
                        //Read disk block b = |PM[2s + 1]| into PM staring at location f2*512: read_block(b, f1*512)
                        int b = abs(PM[2*s + 1]);
                        read_block(b,f1*512);
                        //PM[2s + 1] = f1                              /* update ST entry */
                        PM[2*s + 1]  = f1;
                        // cout << "PM[2* "<< s << "+ 1] = " << PM[2*s + 1]  << endl;

                    }
                    if(PM[PM[2*s + 1]*512 + p] < 0)        /* page fault: page is not resident */
                    {
                        // cout << "page fault: page is not resident" << endl << endl;
                        // cout << "PM[PM[2* "<<s <<" + 1]*512 + "<< p << "] = " <<  PM[PM[2*s + 1]*512 + p] << endl;
                        //Allocate free frame f2 using list of free frames
                        int f2 = nextFree();
                        frames[f2] = 1;
                        // cout << "the free frame is = " << f2 << endl;
                        //Update list of free frames
                        ++next_available_frame;
                        //Read disk block b = |PM[PM[2s + 1]*512 + p]| into PM staring at location f2*512: read_block(b, f2*512)
                        int b = abs(PM[PM[2*s + 1]*512 + p]);
                        read_block(b,f2*512);
                        //PM[2s + 1] = f1                     /* update ST entry */
                        PM[PM[2*s + 1]*512 + p] = f2;         /* update PT entry */
                        // cout << "PM[PM[2* "<< s << "+ 1]*512 + "<< p << "] = " << PM[PM[2*s + 1]*512 + p] << endl;
                
                    }
                    //PA = PM[PM[2s + 1]*512 + p]*512 + w
                    outfile << PM[PM[2*s + 1]*512 + p]*512 + w << " ";
                    // cout << " PM["<<PM[2*s + 1]*512 + p <<"] *512" <<  "+" << w  << " : " << PM[PM[2*s + 1]*512 + p]*512 + w << endl;
                    // cout << "after each command: " << endl;
                    // printPM();
                        
                }
                else if(command == "NL")  {
                    // Failed to read both command and vaddress, indicating "NL" command
                    outfile << endl;
                }
                else if(command == "RP"){
                    int paddress;
                    iss >> paddress;
                    // cout << endl;
                    // cout << "here in RP" << endl;
                    // VA va(vaddress); //create a new VA and assign the integer vaddress 
                    // int pw = va.getPW(); //get the PW
                    // int s = va.getS(); 
                    // int p = va.getP();
                    // int w = va.getW();
                    // cout << vaddress << endl;
                    // cout << "s : " << va.getS() << endl;
                    // cout << "p : " << va.getP() << endl;
                    // cout << "w : " << va.getW() << endl;
                    // cout << "pw : " << va.getPW() << endl;
                    // cout << paddress << endl;
                    if(paddress >= PM_SIZE)
                        outfile << -1 << " ";
                    else{
                        outfile << PM[paddress] << " ";
                        // cout << "PM[ "<<paddress << " ] :  "<< PM[paddress] << endl;
                    }

                }
            }
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

//read_block(b, m) copies the entire block D[b] into the PM frame starting at location PM[m]. 
void read_block(int b, int m){
    for(int i = 0; i< 512; i++){
        PM[m + i] = disk[b][i];
    }

}
int nextFree(){
    for(int i= next_available_frame; i<ST_SIZE; i++){
        if(frames[i] == 0){
            next_available_frame = i;
            return i;
        }
           
    }
    return -1;
}
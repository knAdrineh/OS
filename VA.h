#ifndef VA_H
#define VA_H
#include <string>
using namespace std;

class VA
{
private:
   int addr;  // store the inital integer address given 
   int segment;   // store the segement value
   int page;   // store the page value
   int w_offest;   // store the offest 
   int pw;
public:
    VA(/* args */); //default constructor
    VA(int intVA); //constrocture with initial intge value
    ~VA(); //defualt destructor I don't think I need it here
    void setVA(int intVA); //pass in the Virtual address
    int getIntegerVA(); // get VA in integer
    string getBinaryVA(int intVA); //return the binary VA as a string
    int getInteger(string binaryStr); //return integer value of binary string

    void setS(); //set the segment of VA
    int getS(); //return the segment of VA

    void setP(); //set the page of VA
    int getP(); //return the page of VA

    void setW(); //set the offset of vA
    int getW(); //return the offset of vA

    void setPW();
    int getPW();
};

#endif


#include "VA.h"
#include <cmath>
#include <iostream>
using namespace std;

// Constructor definitions
VA::VA(){}
VA::VA(int intVA) {
    setVA(intVA);
}

// Destructor definition
VA::~VA() {}

// Set the virtual address and calculate the components
void VA::setVA(int intVA) {
    addr = intVA;
    setS();
    setP();
    setW();
    setPW();
}
int VA::getIntegerVA() // get VA in integer
{
    return this->addr;
}
string VA::getBinaryVA(int intVA) //return the binary VA as a string
{
    if (intVA == 0) return "0"; // Special case for zero

    int remainder;
    string binary = "";
    while (intVA > 0) {
        remainder = intVA % 2;
        binary = to_string(remainder) + binary; // Prepend the bit to the binary string
        intVA = intVA / 2;
    }
    // //also make sure the binary thing is  at least 18 so add leading zeros
    while (binary.size() < 18)
    {
        binary = "0" + binary; // Prepend the bit to the binary string
    }

    return binary;
}
int VA::getInteger(string binaryStr) //return integer value of binary string
{
    if (binaryStr.size() == 0 ) return -1;
    int integerValue = 0;
    int power = 0;
    for(int i = binaryStr.size() -1; i >= 0; i--){
        //if there is a one add to the total value
        if(binaryStr[i] == '1')
            integerValue += pow(2, power);
        power++;
    }
    return integerValue;
}
void VA::setS(){
    //s is extracted by right-shifting VA by 18 bits, which discards p and w.
    if(this->addr >= 0) this->segment = this->addr >> 18;
    else this->segment = -1;
     // string bin = getBinaryVA(this->addr);
    // cout << bin << endl;
    // bin = bin.substr(bin.size() - 18);
    // this->segment = getInteger(bin);//if va is 0 or positive

    // this->segment = -1;
}
//based on the VA it returns the segment 
int VA::getS(){

    return this->segment;
}
//based on the VA it returns the page
void VA::setW(){
    // w is extracted by ANDing the VA with the 9-bit binary constant 
   //“1 1111 1111” (or 1FF in hexadecimal), 
   //which removes all bits other than the last 9 bits—the value of w.
    // if(this->addr >= 0){ // if the address is a poistive integer
    // //get the binary string of that 
    // string bin = getBinaryVA(this->addr);
    // //now take the substring which will the last nine digits 
    // string lastNinebits = bin.substr(bin.size() - 9);
    // // now we need to turn the bits to integer
    // this->w_offest= getInteger(lastNinebits);
    // }
    if(this->addr >= 0) this->w_offest = (this->addr & 0x1FF);
    else this->w_offest = -1;
}
int VA::getW(){    
    return this->w_offest;
}
void VA::setP(){
        // p is extracted by first right-shifting VA by
    // 9 bits to discard w. The result is then ANDed with the binary constant 
    //“1 1111 1111,” which removes all bits other than the last 9 bits—the value of p.

    //right shift by 9 bits 
    if(this->addr < 0 ) this->page =-1;
    else page = (addr >> 9) & 0x1FF;
 
}
//based on the VA it returns the offset
int VA::getP(){
    return this->page;
}
void VA::setPW(){
    //pw is extracted by ANDing the VA with the 18-bit binary constant
    // “11 1111 1111 1111 1111” (or 3FFFF in hexadecimal), which removes the leading s
    //right shift by 9 bits 
    if(this->addr< 0 ) this->pw = -1;
    else this->pw = (this->addr & 0x3FFFF);
    // //first get the binray string 
    // string bin = getBinaryVA(this->addr);

    // //now only take the last 18 bits
    // string lastEighteenbits = bin.substr(bin.size() - 18);

    // //now turn it back into a integer
    // this->pw = getInteger(lastEighteenbits);
}
//based on the VA it returns the PW
int VA::getPW(){
    return this->pw;
}
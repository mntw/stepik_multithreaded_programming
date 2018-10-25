#include <iostream>
#include <string.h>


int main() {
    char Memory[1048576];
    size_t * p = (size_t *) &Memory;
    char String[] = "All your bases are belongs to us...\0";
    p[0] = 0;
    p[1] = sizeof(String);
    strcpy(&Memory[16], String);
    p[0] =  (size_t) &Memory[8 + 8 + p[1]];
    size_t * q = (size_t *) p[0];
    * (size_t *) p[0] = 0;
    q[1] = 1; 
    std::cout << "Address of the array\t " << &Memory << std::endl;
    std::cout << "Address of the pointer p[0]\t " << p <<" value " << std::hex << p[0] << std::endl;
    std::cout << "Address of the pointer p[1]\t " << &p[1] <<" value "<< std::dec <<p[1]<< std::endl;
    for (size_t i = 0; i != sizeof(String); ++i){
        std::cout << Memory[16 + i];
    }
    std::cout << std::endl;
    std::cout << "Address of the pointer q[0]\t "  << q << " value " << q[0] << std::endl;
    std::cout << "Address of the pointer q[1]\t "  << &q[1] << " value " << q[1] << std::endl;
    return 0;
}

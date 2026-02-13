#include <iostream>
#include <cstdlib>
#include "adder.h"
int main(int argc, char* argv[]) {
    int a = std::atoi(argv[1]);
    int b = std::atoi(argv[2]);

    std::cout << add(a, b) << std::endl;  // add() not defined yet on purpose
    return 0;
}
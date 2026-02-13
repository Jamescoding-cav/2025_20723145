#include <iostream>
#include <cstdlib>
#include "adder.h"

int main(int argc, char* argv[]) {
    int a = 0, b = 0;

    if (argc >= 3) {
        a = std::atoi(argv[1]);
        b = std::atoi(argv[2]);
    }
    else {
        std::cout << "Enter two integers: ";
        std::cin >> a >> b;
    }

    std::cout << add(a, b) << std::endl;
    return 0;
}

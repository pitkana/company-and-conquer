#define DEBUG 1 //set to 0 to disable debug features

#include <iostream>
#include "backend/helper_tools.hpp"

int main() 
{
    helper::Matrix<size_t> a(5, 5, 0);
    a(5, 10) = 3;
    std::cout << a(5, 1) << std::endl;

    return 0;
}

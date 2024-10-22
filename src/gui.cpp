#include <iostream>
#include "backend/helper_tools.hpp"

int main() 
{
    std:: cout << helper::clamp<int>(5, 0, 6) << "\n";
    helper::Matrix<size_t> a(5, 5, 0);


    return 0;
}
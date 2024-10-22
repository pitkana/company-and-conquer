#define DEBUG 1 //set to 0 to disable debug features

#include <iostream>
#include "backend/helper_tools.hpp"
#include <memory>


int main() 
{
    std:: cout << helper::clamp<int>(5, 0, 6) << "\n";
    helper::Matrix<size_t> a(5, 5, 0);


    std::weak_ptr<int> b;
    {
        std::shared_ptr<int> o = std::make_shared<int>(5); 
        b = o;
    }
    
    

    return 0;
}

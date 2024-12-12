#include "file_selector_test.hpp"

#include <iostream>
#include <ostream>
#include <tinyfiledialogs.h>

int file_selector_test()
{
    char const * lFilterPatterns[1] = { "*.yaml" };
    auto selection = tinyfd_openFileDialog("Select file", "", 1, lFilterPatterns, NULL, 0);
    std::cout << selection << std::endl;
    return 0;
}

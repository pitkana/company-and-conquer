#include "easter_egg_test.hpp"
#include "name_gen.hpp"

#include <string>
#include <iostream>

int easter_egg_test()
{
    NameGen name_gen;
    std::string name;
    int count = 0;
    while (name != "Pvt. Parts")
    {
        count++;
        name = name_gen.generate(name_lists.english, 0, 4);
    }
    std::cout << "Found Pvt Parts on try " << count << std::endl;
}

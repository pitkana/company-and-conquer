#include "name_gen.hpp"

#include <random>
#include <string>
#include <ctime>

NameGen::NameGen()
{
    srand(time(0));
}

std::string NameGen::generate(std::vector<std::string> names, int min_rank, int max_rank) const
{
    if (rand() % 1000 == 0)
    {
        return "Pvt. Parts";
    }
    int name = rand() % names.size();
    int rank = rand() % (max_rank + 1 - min_rank) + min_rank;

    std::string name_and_rank = ranks[rank] + " " + names[name];

    return name_and_rank;
}

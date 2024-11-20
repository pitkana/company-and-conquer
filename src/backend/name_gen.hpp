#pragma once

#include <vector>
#include <array>
#include <string>

inline struct
{
    std::vector<std::string> english = {"Jenkins", "Atkins", "Wilkins", "Roberts", "Smith", "Lewis", "Chapman", "Knight", "Hughes", "Clarke"};
    std::vector<std::string> german = {"Schmidt", "Müller", "Leer", "Koch", "Klein", "Schneider", "Schultz", "König", "Herrmann", "Lorenz"};
} name_lists;

inline std::array<std::string, 5> ranks = {"Pvt.", "Cpl.", "Sgt.", "Lt.", "Cpt."};

class NameGen
{
public:
    NameGen();

    std::string generate(std::vector<std::string> names, int min_rank, int max_rank) const;
};

#include "backend/unit.hpp"
#include <iostream>
#include <string>

int main() 
{
	std::string name = "Test Unit";
	Unit testUnit = Unit(name);
	std::cout << testUnit.GetName() << std::endl;
	std::cout << testUnit.GetHP() << std::endl;

	return 0;
}

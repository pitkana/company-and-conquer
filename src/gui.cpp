#define DEBUG 1 //set to 0 to disable debug features

#include <iostream>
#include "backend/helper_tools.hpp"
#include <memory>
#include <SFML/Window.hpp>

int main() 
{
    helper::Matrix<size_t> a(5,5,0);
    a(10, 10);
    sf::Window window(sf::VideoMode(800, 600), "My window");
    
// run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}

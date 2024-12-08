#include <iostream>
#include <memory>
#include <SFML/Window.hpp>

#include "coordinates.hpp"
#include "game.hpp"
#include "action.hpp"
#include "coordinates.hpp"
#include <random>

#include "renderer.hpp"





int main()
{
    //start(1000, 1000);
    Renderer renderer(1000, 1000);
    renderer.initialise_level(0);
    renderer.start();
    

    return 0;
}

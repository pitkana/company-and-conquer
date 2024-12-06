#include "renderer.hpp""

//Use WASD to move map and FG to zoom in and out.
//Clicking tile will print the coordinates of that tile.
void rendering_engine_test() {
    Renderer renderer(700,700);
    renderer.initialise_level(0);
    renderer.start();
}
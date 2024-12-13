
#include "integration_test.hpp"

#include "renderer.hpp"

int integration_test() {
    Renderer renderer = Renderer(1200, 700);
    renderer.initialize_scenario();
    return 0;
}

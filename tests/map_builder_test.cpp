#include "map_builder.hpp"
#include "map_builder_test.hpp"


//TODO: Add better tests.
void map_builder_test() {
    Map_Builder builder = Map_Builder();

    Map test_map = builder.load(TESTMAP_PATH);
    test_map.print_map();
}

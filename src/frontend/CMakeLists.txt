file (GLOB FRONTEND_SOURCES_CPP "*.cpp")
file (GLOB FRONTEND_SOURCES_HPP "*.hpp")
file (GLOB FRONTEND_SOURCES_C "*.c")
file (GLOB FRONTEND_SOURCES_H "*.h")

set(FRONTEND_SOURCES ${FRONTEND_SOURCES_H} ${FRONTEND_SOURCES_C} ${FRONTEND_SOURCES_HPP} ${FRONTEND_SOURCES_CPP})

#target_sources(backend PRIVATE ${FRONTEND_SOURCES})
add_library(frontend STATIC ${FRONTEND_SOURCES})

target_link_libraries(frontend PRIVATE backend)

target_link_libraries(frontend PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

target_include_directories(frontend PRIVATE ${SFML_SOURCE_DIR}/include)

# For relative paths.
    # Maps
set(TESTMAP_PATH ${CMAKE_SOURCE_DIR}/tests/test_map.txt)
add_compile_definitions(TESTMAP_PATH="${TESTMAP_PATH}")
set(TESTMAP_PATH1 ${CMAKE_SOURCE_DIR}/tests/test_map1.txt)
add_compile_definitions(TESTMAP_PATH1="${TESTMAP_PATH1}")
set(TEXTURE_PATH ${CMAKE_SOURCE_DIR}/textures/tiles.png)
add_compile_definitions(TEXTURE_PATH="${TEXTURE_PATH}")
    # Units
set(UNITS_TEXTURE_PATH ${CMAKE_SOURCE_DIR}/textures/units.png)
add_compile_definitions(UNITS_TEXTURE_PATH="${UNITS_TEXTURE_PATH}")
    # Buildings
set(BUILDINGS_TEXTURE_PATH ${CMAKE_SOURCE_DIR}/textures/buildings.png)
add_compile_definitions(BUILDINGS_TEXTURE_PATH="${BUILDINGS_TEXTURE_PATH}")
    
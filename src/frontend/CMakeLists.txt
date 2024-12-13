file (GLOB FRONTEND_SOURCES_CPP "*.cpp")
file (GLOB FRONTEND_SOURCES_HPP "*.hpp")
file (GLOB FRONTEND_SOURCES_C "*.c")
file (GLOB FRONTEND_SOURCES_H "*.h")

set(FRONTEND_SOURCES ${FRONTEND_SOURCES_H} ${FRONTEND_SOURCES_C} ${FRONTEND_SOURCES_HPP} ${FRONTEND_SOURCES_CPP})

#target_sources(backend PRIVATE ${FRONTEND_SOURCES})
add_library(frontend STATIC ${FRONTEND_SOURCES})

target_link_libraries(frontend PRIVATE backend)

target_link_libraries(frontend PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(frontend PRIVATE SFMLButton)
target_link_libraries(frontend PRIVATE yaml-cpp::yaml-cpp)

target_include_directories(frontend PRIVATE ${SFML_SOURCE_DIR}/include)
target_include_directories(frontend PRIVATE ${CMAKE_SOURCE_DIR}/libs/SFMLButton/include)
target_include_directories(frontend PRIVATE ${CMAKE_SOURCE_DIR}/libs/tinyfiledialogs)

target_sources(frontend PRIVATE ${CMAKE_SOURCE_DIR}/libs/tinyfiledialogs/tinyfiledialogs.c)



# For relative paths.
    # Maps
set(TESTMAP_PATH ${CMAKE_SOURCE_DIR}/tests/test_map.txt)
add_compile_definitions(TESTMAP_PATH="${TESTMAP_PATH}")
set(TESTMAP_PATH1 ${CMAKE_SOURCE_DIR}/tests/test_map1.txt)
add_compile_definitions(TESTMAP_PATH1="${TESTMAP_PATH1}")
set(TEXTURE_PATH ${CMAKE_SOURCE_DIR}/graphics/terrain.png)
add_compile_definitions(TEXTURE_PATH="${TEXTURE_PATH}")
    # Units
set(UNITS_TEXTURE_PATH ${CMAKE_SOURCE_DIR}/graphics/units.png)
add_compile_definitions(UNITS_TEXTURE_PATH="${UNITS_TEXTURE_PATH}")
    # Buildings
set(BUILDINGS_TEXTURE_PATH ${CMAKE_SOURCE_DIR}/graphics/buildings.png)
add_compile_definitions(BUILDINGS_TEXTURE_PATH="${BUILDINGS_TEXTURE_PATH}")
    # Aux
set(AUX_TEXTURE_PATH ${CMAKE_SOURCE_DIR}/graphics/_aux.png)
add_compile_definitions(AUX_TEXTURE_PATH="${AUX_TEXTURE_PATH}")

set(INVENTORY_TEXTURE_PATH ${CMAKE_SOURCE_DIR}/graphics/inventory.png)
add_compile_definitions(INVENTORY_TEXTURE_PATH="${INVENTORY_TEXTURE_PATH}")

set(SCENARIOS_PATH ${CMAKE_SOURCE_DIR}/scenarios/)
add_compile_definitions(SCENARIOS_PATH="${SCENARIOS_PATH}")

    # Font
set(FONT_PATH ${CMAKE_SOURCE_DIR}/fonts/font.TTF)
add_compile_definitions(FONT_PATH="${FONT_PATH}")
set(GUI_FONT_PATH ${CMAKE_SOURCE_DIR}/fonts/NotoSans-Bold.ttf)
add_compile_definitions(GUI_FONT_PATH="${GUI_FONT_PATH}")

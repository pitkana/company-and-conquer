project(test VERSION 0.1.0)
enable_language(CXX)

# Add all .c, - files under src/ directory
file(GLOB SOURCES_C "*.c")
# Add all .cpp, - files under src/ directory
file(GLOB SOURCES_CPP "*.cpp")
# Add all .hpp, - files under src/ directory
file(GLOB SOURCES_HPP "*.hpp")
# Add all .h, - files under src/ directory
file(GLOB SOURCES_H "*.h")
set(SOURCES ${SOURCES_C} ${SOURCES_CPP} ${SOURCES_H} ${SOURCES_HPP})

# add the src directory
include_directories(${CMAKE_SOURCE_DIR}/src)

# add the executable target
add_executable(test
        scenario_test.cpp
        shop_ui_test.cpp
        shop_ui_test.hpp)

target_sources(test PUBLIC ${SOURCES})
target_include_directories(test PUBLIC .)
target_include_directories(test PUBLIC ../src/)

target_link_libraries(test PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
target_link_libraries(test PRIVATE sfml-graphics)
target_link_libraries(test PRIVATE yaml-cpp::yaml-cpp)

target_compile_features(test PRIVATE cxx_std_17)
target_link_libraries(test PRIVATE backend)
target_include_directories(test PRIVATE ../src/backend)
set_target_properties(backend PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test PRIVATE frontend)
target_include_directories(test PRIVATE ../src/frontend)
set_target_properties(frontend PROPERTIES LINKER_LANGUAGE CXX)

# For relative paths.
    # Maps
    set(TESTMAP_PATH ${CMAKE_SOURCE_DIR}/tests/test_map.txt)
    add_compile_definitions(TESTMAP_PATH="${TESTMAP_PATH}")
    set(TESTMAP_PATH1 ${CMAKE_SOURCE_DIR}/tests/test_map1.txt)
    add_compile_definitions(TESTMAP_PATH1="${TESTMAP_PATH1}")
    set(TEXTURE_PATH ${CMAKE_SOURCE_DIR}/textures/tiles.png)
    add_compile_definitions(TEXTURE_PATH="${TEXTURE_PATH}")
        # Units
    set(UNITS_TEXTURE ${CMAKE_SOURCE_DIR}/textures/units.png)
    add_compile_definitions(UNITS_TEXTURE="${TEXTURE_PATH}")
    

# get a lot of warnings
if(MSVC)
    target_compile_options(test PRIVATE -std=c++20 /Wall)
else()
    # gcc and clang
    target_compile_options(test PRIVATE -std=c++20 -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()

if(WIN32)
    add_custom_command(
        TARGET test
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:test>
        VERBATIM)
endif()

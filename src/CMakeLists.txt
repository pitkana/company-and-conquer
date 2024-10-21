cmake_minimum_required(VERSION 3.11.0)
project(gui VERSION 0.1.0)

enable_language(CXX)

# use C++ standard version 20
set(CMAKE_CXX_STANDARD 20)
# produce non-optimized code with debug symbols
set(CMAKE_BUILD_TYPE Debug)


message(STATUS "BUILD directory is ${CMAKE_BINARY_DIR}")
message(STATUS "SRC directory is ${CMAKE_SOURCE_DIR}")


# Add all .c, - files under src/ directory
file(GLOB SOURCES_C "*.c")
# Add all .cpp, - files under src/ directory
file(GLOB SOURCES_CPP "*.cpp")
# Add all .hpp, - files under src/ directory
file(GLOB SOURCES_HPP "*.hpp")
# Add all .h, - files under src/ directory
file(GLOB SOURCES_H "*.h")
set(SOURCES ${SOURCES_C} ${SOURCES_CPP} ${SOURCES_H} ${SOURCES_HPP})

# add the executable target
add_executable(gui.exe)

target_sources(gui.exe PUBLIC ${SOURCES})
target_include_directories(gui.exe PUBLIC .)
target_include_directories(gui.exe PUBLIC src)




# get a lot of warnings
if(MSVC)
    target_compile_options(gui.exe PRIVATE -std=c++20 /Wall)
else()
    # gcc and clang
    target_compile_options(gui.exe PRIVATE -std=c++20 -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()

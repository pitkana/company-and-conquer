cmake_minimum_required(VERSION 3.11.0)
project(unit_test VERSION 0.1.0)

enable_language(CXX)

# use C++ standard version 20
set(CMAKE_CXX_STANDARD 20)
# produce non-optimized code with debug symbols
# set(CMAKE_BUILD_TYPE Debug)

# Generate compile_commands.json file for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

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

# add the src directory
include_directories(${CMAKE_SOURCE_DIR}/src)

# add the executable target
add_executable(unit_test)

target_sources(unit_test PUBLIC ${SOURCES})
target_include_directories(unit_test PUBLIC .)
target_include_directories(unit_test PUBLIC ../src/)
target_link_libraries(unit_test PRIVATE sfml-graphics)
target_compile_features(unit_test PRIVATE cxx_std_17)


# get a lot of warnings
if(MSVC)
    target_compile_options(unit_test PRIVATE -std=c++20 /Wall)
else()
    # gcc and clang
    target_compile_options(unit_test PRIVATE -std=c++20 -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()

if(WIN32)
    add_custom_command(
        TARGET unit_test
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:unit_test>
        VERBATIM)
endif()

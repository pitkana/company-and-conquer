cmake_minimum_required(VERSION 3.11.0)
project(PROJECT VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_subdirectory(src)
add_subdirectory(tests)


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

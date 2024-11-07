project(unit_test VERSION 0.1.0)
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
add_executable(unit_test)

target_sources(unit_test PUBLIC ${SOURCES})
target_include_directories(unit_test PUBLIC .)
target_include_directories(unit_test PUBLIC ../src/)
target_link_libraries(unit_test PRIVATE sfml-graphics)
target_link_libraries(unit_test PRIVATE backend)
target_include_directories(unit_test PRIVATE ../src/backend)


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

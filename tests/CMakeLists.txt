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
add_executable(test)

target_sources(test PUBLIC ${SOURCES})
target_include_directories(test PUBLIC .)
target_include_directories(test PUBLIC ../src/)
target_link_libraries(test PRIVATE sfml-graphics)
target_compile_features(test PRIVATE cxx_std_17)
target_link_libraries(test PRIVATE backend)
target_include_directories(test PRIVATE ../src/backend)


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

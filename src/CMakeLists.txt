project(gui VERSION 0.1.0)
enable_language(CXX)

add_subdirectory(backend)

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
add_executable(gui)

target_sources(gui PUBLIC ${SOURCES})
target_link_libraries(gui PRIVATE sfml-graphics)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(gui PRIVATE backend)
target_include_directories(gui PRIVATE backend)


# get a lot of warnings
if(MSVC)
    target_compile_options(gui PRIVATE -std=c++20 /Wall)
else()
    # gcc and clang
    target_compile_options(gui PRIVATE -std=c++20 -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()

if(WIN32)
    add_custom_command(
        TARGET gui
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:gui>
        VERBATIM)
endif()

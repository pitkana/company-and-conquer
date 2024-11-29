file (GLOB BACKEND_SOURCES_CPP "*.cpp")
file (GLOB BACKEND_SOURCES_HPP "*.hpp")
file (GLOB BACKEND_SOURCES_C "*.c")
file (GLOB BACKEND_SOURCES_H "*.h")

set(BACKEND_SOURCES ${BACKEND_SOURCES_H} ${BACKEND_SOURCES_C} ${BACKEND_SOURCES_HPP} ${BACKEND_SOURCES_CPP})

# target_sources(gui PRIVATE ${BACKEND_SOURCES})
add_library(backend STATIC ${BACKEND_SOURCES})
target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(backend PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

target_include_directories(backend PRIVATE ${SFML_SOURCE_DIR}/include)
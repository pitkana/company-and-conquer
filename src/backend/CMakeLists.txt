file(GLOB BACKEND_SOURCES_CPP "*.cpp")
file(GLOB BACKEND_SOURCES_HPP "*.hpp")
file(GLOB BACKEND_SOURCES_C "*.c")
file(GLOB BACKEND_SOURCES_H "*.h")

set(BACKEND_SOURCES ${BACKEND_SOURCES_H} ${BACKEND_SOURCES_C} ${BACKEND_SOURCES_HPP} ${BACKEND_SOURCES_CPP})

add_library(backend STATIC ${BACKEND_SOURCES})

include(FetchContent)

# Fetch yaml-cpp
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

# Link yaml-cpp
target_link_libraries(backend PRIVATE yaml-cpp::yaml-cpp)
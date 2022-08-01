cmake_minimum_required(VERSION 3.10)

project(swordfish VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -Wall)

add_subdirectory(sfutils)

configure_file(config.hpp.in config.hpp)
add_executable(swordfish main.cpp)

target_link_libraries(swordfish PUBLIC
    sfutils
)
target_include_directories(swordfish PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/sfutils"
)

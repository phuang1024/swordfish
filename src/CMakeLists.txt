cmake_minimum_required(VERSION 3.10)

project(swordfish VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -Wall)

add_subdirectory(sfeval)
add_subdirectory(sfmovegen)
add_subdirectory(sfsearch)
add_subdirectory(sfutils)

configure_file(config.hpp.in config.hpp)
add_executable(swordfish main.cpp)

target_link_libraries(swordfish PUBLIC
    sfeval
    sfmovegen
    sfsearch
    sfutils
)
target_include_directories(swordfish PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/sfeval"
    "${PROJECT_SOURCE_DIR}/sfmovegen"
    "${PROJECT_SOURCE_DIR}/sfsearch"
    "${PROJECT_SOURCE_DIR}/sfutils"
)

cmake_minimum_required(VERSION 3.10)

project(swordfish VERSION 0.0.1)

configure_file(config.hpp.in config.hpp)
add_executable(swordfish main.cpp)

target_include_directories(swordfish PUBLIC "${PROJECT_BINARY_DIR}")

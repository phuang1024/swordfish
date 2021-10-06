#
#  Swordfish
#  A chess engine.
#  Copyright Patrick Huang 2021
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Config stuff
cmake_minimum_required(VERSION 3.10)

find_package(Git)

# Project
project(Swordfish VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
set(Swordfish_srcs main.cpp)
add_executable(Swordfish ${Swordfish_srcs})

# Configure files
execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --match=asdf --always --abbrev=40 --dirty
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_HASH
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(config.hpp.in config.hpp)

# Add libraries

#set(submods "")
#list(APPEND submods)

#foreach(submod ${submods})
#    add_subdirectory(${submod})
#endforeach()


# Link and include
target_link_libraries(Swordfish
    #argparse gui logging pathlib render space3d tests utils
)
target_include_directories(Swordfish PUBLIC "${PROJECT_SOURCE_DIR}")

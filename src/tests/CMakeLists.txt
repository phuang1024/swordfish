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

set(tests_srcs position.cpp utils.cpp)
add_executable(tests ${tests_srcs})

target_link_libraries(tests PUBLIC
    position
    utils
)

# Setup Catch2
add_subdirectory(../../3rdparty/Catch2 ../catch2)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# CMake tests
add_test(NAME all COMMAND tests)
if(WITH_TESTING)
    enable_testing()
endif()

//
//  Swordfish
//  A chess engine.
//  Copyright Patrick Huang 2021
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <iostream>

#include "position.hpp"
#include "utils.hpp"
#include "tests.hpp"


namespace Tests {


void testall() {
    for (const std::string& name: tests)
        test(name);
}

void test(const std::string name) {
    if      (name == "assert") test_assert();
    else if (name == "pos_methods") test_pos_methods();
    else if (name == "pos_helpers") test_pos_helpers();
    else if (name == "pos_fen") test_pos_fen();
    else if (name == "pos_movegen") test_pos_movegen();
    else {
        std::cerr << "# Unrecognized test: " << name << std::endl;
        throw Errors::InvalidArg;
    }
}


}  // namespace Tests

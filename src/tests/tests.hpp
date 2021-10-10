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

#include <string>
#include <vector>


namespace Tests {

const std::vector<std::string> tests = {
    "bb_attacks",
};


/**
 * Test generating attacks.
 */
void bb_attacks();


/**
 * Run all tests.
 */
void testall();

/**
 * Run test with name.
 */
void test(const std::string name);


}  // namespace Tests

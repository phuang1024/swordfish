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

/**
 * List of the names of all tests.
 * Pass this as a cli arg to run the test.
 * e.g. ./Swordfish test my_test_name
 */
const std::vector<std::string> tests = {
    "fen",

};


// General tests (general.cpp)

/**
 * Test the assert() macro (defined in utils.cpp)
 * This is the only test NOT added to the tests list because it always fails.
 */
void test_assert();


// Chess position (position.cpp)

/**
 * Test generating and parsing FENs.
 */
void test_fen();


/**
 * Run all tests.
 */
void testall();

/**
 * Run test with name.
 */
void test(const std::string name);


}  // namespace Tests

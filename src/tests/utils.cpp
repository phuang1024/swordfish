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

#include <catch2/catch_test_macros.hpp>

#include "utils.hpp"


TEST_CASE("Utils: Bit processing", "[utils.bitproc]") {
    SECTION("bit") {
        REQUIRE(bit(123, 0));
        REQUIRE(!bit(123, 2));
        REQUIRE(nbit(123, 2));
        REQUIRE(!nbit(123, 0));
    }

    SECTION("bset") {
        REQUIRE(bset(123, 2) == 127);
        REQUIRE(bset(123, 0) == 123);
        REQUIRE(bunset(127, 2) == 123);
        REQUIRE(bunset(125, 0) == 124);
    }

    SECTION("bpos") {
        REQUIRE(bpos(128) == 7);
        REQUIRE(bpos(4096) == 12);
    }

    SECTION("popcnt") {
        REQUIRE(popcnt(0ULL) == 0);
        REQUIRE(popcnt(3279492ULL) == 7);
    }

    SECTION("in_board") {
        REQUIRE(in_board(7, 3));
        REQUIRE(!in_board(4, 8));
    }

    SECTION("square") {
        REQUIRE(square(7, 3) == 7 + 3*8);
        REQUIRE(square(2, 5) == 2 + 5*8);
    }
}

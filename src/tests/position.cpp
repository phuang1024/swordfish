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

#include "position.hpp"


TEST_CASE("Position: Helper methods", "[pos_helpers]") {
    REQUIRE(Position::char2piece('P') == Position::WP);
    REQUIRE(Position::char2piece('B') == Position::WB);
    REQUIRE(Position::char2piece('n') == Position::BN);
    REQUIRE(Position::char2piece('q') == Position::BQ);

    REQUIRE(Position::piece2char(Position::WP));
    REQUIRE(Position::piece2char(Position::WB));
    REQUIRE(Position::piece2char(Position::BN));
    REQUIRE(Position::piece2char(Position::BQ));

    REQUIRE(Position::sq2alg(0) == "a1");
    REQUIRE(Position::sq2alg(20) == "e3");
    REQUIRE(Position::sq2alg(33) == "b5");
    REQUIRE(Position::sq2alg(53) == "f7");

    REQUIRE(Position::alg2sq("a1") == 0);
    REQUIRE(Position::alg2sq("e3") == 20);
    REQUIRE(Position::alg2sq("b5") == 33);
    REQUIRE(Position::alg2sq("f7") == 53);

    REQUIRE(Position::promo2char(0) == 'N');
    REQUIRE(Position::promo2char(1) == 'B');
    REQUIRE(Position::promo2char(2) == 'R');
    REQUIRE(Position::promo2char(3) == 'Q');

    REQUIRE(Position::char2promo('N') == 0);
    REQUIRE(Position::char2promo('B') == 1);
    REQUIRE(Position::char2promo('R') == 2);
    REQUIRE(Position::char2promo('Q') == 3);
}

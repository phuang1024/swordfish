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


TEST_CASE("Position: Move generation", "[pos_movegen]") {
    REQUIRE(Position::bb_ray(9, 36) == 68853957120ULL);
    REQUIRE(Position::bb_ray(13, 37) == 137977929728ULL);

    Position::Position pos1("8/3r4/4B3/1P6/2K2R2/7P/4Nk2/8 w - - 0 1");
    Position::Position pos2("8/8/8/p7/2q1K3/6p1/2R5/8 b - - 0 1");
    Position::RespectivePieces resp1(pos1);
    Position::RespectivePieces resp2(pos2);
    REQUIRE(Position::attacked(pos1, resp1.SAME, resp1.OTHER, true, false) == 6928828910485250116ULL);
    REQUIRE(Position::attacked(pos1, resp1.SAME, resp1.OTHER, true, true) == 6928828910502027620ULL);
    REQUIRE(Position::attacked(pos2, resp2.SAME, resp2.OTHER, false, false) == 4910072644068357408ULL);
    REQUIRE(Position::attacked(pos2, resp2.SAME, resp2.OTHER, false, true) == 4910072647826453792ULL);

    Position::Position pos3("8/8/2B5/3kP1R1/1BP5/4N3/8/3Q4 b - - 0 1");
    Position::Position pos4("8/3p4/rNK1n3/8/2r1b3/8/2q5/8 w - - 0 1");
    Position::RespectivePieces resp3(pos3);
    Position::RespectivePieces resp4(pos4);
    REQUIRE(Position::checkers(pos3, false, 3, 4, resp3) == 4398114668552ULL);
    REQUIRE(Position::checkers(pos4, true, 2, 5, resp4) == 2251800149229568ULL);
}

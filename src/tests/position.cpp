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


void test_fen_check(const Position::Position& pos, ULL wp, ULL wn, ULL wb, ULL wr, ULL wq, ULL wk,
ULL bp, ULL bn, ULL bb, ULL br, ULL bq, ULL bk, UCH meta, UCH ep, USH moveclock, USH fullmoves) {
    bool correct = pos.wp == wp && pos.wn == wn && pos.wb == wb && pos.wr == wr && pos.wq == wq &&
        pos.wk == wk && pos.bp == bp && pos.bn == bn && pos.bb == bb && pos.bq == bq && pos.bk == bk &&
        pos.meta == meta && pos.ep == ep && pos.moveclock == moveclock && pos.fullmoves == fullmoves;

    if (!correct)
        Position::print(std::cerr, pos);
    assert(correct);
}

void test_fen() {
    std::cerr << "### Test fen\n";

    std::cerr << "Parsing FENs:\n";

    const std::string fen1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const Position::Position pos1(fen1);
    std::cerr << "Fen 1: " << fen1;
    test_fen_check(pos1, 65280ULL, 66ULL, 36ULL, 129ULL, 8ULL, 16ULL, 71776119061217280ULL,
        4755801206503243776ULL, 2594073385365405696ULL, 9295429630892703744ULL, 576460752303423488ULL,
        1152921504606846976ULL, 31, 0, 0, 1);
    std::cerr << " Passed\n";
}


}  // namespace Tests

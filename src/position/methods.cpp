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

/**
 * General helper methods.
 */

#include <iostream>
#include <string>

#include "position.hpp"


namespace Position {


char piece2char(char piece) {
    switch (piece) {
        case WP: return 'P';
        case WN: return 'N';
        case WB: return 'B';
        case WR: return 'R';
        case WQ: return 'Q';
        case WK: return 'K';
        case BP: return 'p';
        case BN: return 'n';
        case BB: return 'b';
        case BR: return 'r';
        case BQ: return 'q';
        case BK: return 'k';
        case EMPTY: return ' ';
    }
    throw Errors::InvalidArg;
}

char char2piece(char piece) {
    switch (piece) {
        case 'P': return WP;
        case 'N': return WN;
        case 'B': return WB;
        case 'R': return WR;
        case 'Q': return WQ;
        case 'K': return WK;
        case 'p': return BP;
        case 'n': return BN;
        case 'b': return BB;
        case 'r': return BR;
        case 'q': return BQ;
        case 'k': return BK;
        case ' ': return EMPTY;
    }
    throw Errors::InvalidArg;
}


void Position::print(std::ostream& fp) {
    // Design from stockfish.
    const std::string row = " +---+---+---+---+---+---+---+---+";
    const std::string col = " | ";
    const std::string columns = "  a   b   c   d   e   f   g   h";

    for (int y = 7; y >= 0; y--) {
        std::cout << row << '\n';
        std::cout << col;
        for (int x = 0; x < 8; x++) {
            const char piece = piece_at(y*8 + x);
            const char symbol = piece2char(piece);
            std::cout << symbol << col;
        }
        std::cout << y+1 << '\n';
    }
    std::cout << row << "\n" << columns << std::endl;
}

char Position::piece_at(const char square) {
    if (bit(wp, square)) return WP;
    if (bit(wn, square)) return WN;
    if (bit(wb, square)) return WB;
    if (bit(wr, square)) return WR;
    if (bit(wq, square)) return WQ;
    if (bit(wk, square)) return WK;
    if (bit(bp, square)) return BP;
    if (bit(bn, square)) return BN;
    if (bit(bb, square)) return BB;
    if (bit(br, square)) return BR;
    if (bit(bq, square)) return BQ;
    if (bit(bk, square)) return BK;
    return EMPTY;
}


}  // namespace Position

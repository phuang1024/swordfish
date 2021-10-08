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

#include "../utils.hpp"


namespace Position {


// Constants

constexpr char EMPTY = 0;
constexpr char WP = 1, WN = 2, WB = 3, WR = 4, WQ = 5, WK = 6;
constexpr char BP = 9, BN = 10, BB = 11, BR = 12, BQ = 13, BK = 14;


// Position and movegen

/**
 * Get character representation of a piece.
 * piece2char(Position::WP) -> 'P'
 * @throws Errors::InvalidArg
 */
char piece2char(char piece);

/**
 * Get piece for char representation.
 * char2piece('P') -> Position::WP
 * @throws Errors::InvalidArg
 */
char char2piece(char piece);

/**
 * Represents one position on a chessboard.
 * Contains bitboards (ULL) for each piece type and color.
 * Contains turn (bool).
 * Contains castling rights (char).
 *
 * Square values: A1 = 0, A2 = 1, A8 = 7, B1 = 8, H8 = 63
 */
struct Position {
    ULL wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;
    bool turn;     // true = white
    UCH castling;  // not decided yet.

    /**
     * Write board to stream in a human readable way.
     */
    void print(std::ostream& fp);

    /**
     * Return the piece at the position specified.
     * e.g. WP, BR, EMPTY
     */
    char piece_at(const char square);
};


}  // namespace Position

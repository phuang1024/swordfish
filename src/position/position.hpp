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
#include <vector>

#include "../utils.hpp"


namespace Position {


// Constants

// Piece values
constexpr char EMPTY = 0;
constexpr char WP = 9, WN = 10, WB = 11, WR = 12, WQ = 13, WK = 14;
constexpr char BP = 1, BN = 2, BB = 3, BR = 4, BQ = 5, BK = 6;

// Starting bitboards
constexpr ULL START_WP = 65280ULL;
constexpr ULL START_WN = 66ULL;
constexpr ULL START_WB = 36ULL;
constexpr ULL START_WR = 129ULL;
constexpr ULL START_WQ = 8ULL;
constexpr ULL START_WK = 16ULL;
constexpr ULL START_BP = 71776119061217280ULL;
constexpr ULL START_BN = 4755801206503243776ULL;
constexpr ULL START_BB = 2594073385365405696ULL;
constexpr ULL START_BR = 9295429630892703744ULL;
constexpr ULL START_BQ = 576460752303423488ULL;
constexpr ULL START_BK = 1152921504606846976ULL;

// Castling constants (use bitwise "and" on Position.meta)
constexpr UCH C_WK = 1;
constexpr UCH C_WQ = 2;
constexpr UCH C_BK = 4;
constexpr UCH C_BQ = 8;

// Other
constexpr UCH TURN = 16;  // bitwise "and" on Position.meta


// Helpers

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


// Position

/**
 * Represents one position on a chessboard.
 * Contains bitboards (ULL) for each piece type and color.
 * Contains turn (bool).
 * Contains castling rights (char).
 *
 * Square values: A1 = 0, A2 = 1, A8 = 7, B1 = 8, H8 = 63
 */
struct Position {
    /**
     * Pass to constructor to initialize as empty.
     */
    const static char INIT_EMPTY = 0;

    /**
     * Pass to constructor to initialize as starting position.
     */
    const static char INIT_START = 1;

    /**
     * Piece bitboards.
     */
    ULL wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;

    /**
     * Metadata containing:
     * * castling (4 bits), WK, WQ, BK, BQ
     * * turn (1 bit), true = white
     */
    UCH meta;

    /**
     * No initialization.
     * Warning: Member variables may be arbitrary.
     */
    Position();

    /**
     * Initialize with init code (e.g. Position::INIT_START)
     */
    Position(const char code);

    /**
     * Return the piece at the position specified.
     * e.g. WP, BR, EMPTY
     */
    char piece_at(const char square) const;
};

/**
 * Move struct, containing start and end square.
 * Position is responsible for detecting castling and en passant.
 */
struct Move {
    UCH from, to;
};

/**
 * Write board to stream in a human readable way.
 */
void print(std::ostream& fp, const Position& pos);

/**
 * Write bitboard to stream in a human readable way.
 */
void print(std::ostream& fp, const ULL board);


// Movegen

/**
 * Get bitboard of attacked pieces.
 * @param side true = white's attacks.
 */
ULL attacked(const Position& pos, const bool side);

/**
 * Generate legal moves and store in @param moves.
 */
void legal_moves(std::vector<Move>& moves, const Position& pos);


}  // namespace Position

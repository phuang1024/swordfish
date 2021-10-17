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

#include "utils.hpp"


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

// Position metadata

// Castling constants (use bitwise "and" on Position.meta)
constexpr UCH C_WK = 1;
constexpr UCH C_WQ = 2;
constexpr UCH C_BK = 4;
constexpr UCH C_BQ = 8;

// Bitwise "and" on Position.meta
constexpr UCH TURN = 16;

// Bitwise "and" on Position.ep
constexpr UCH EP_YES = 64;  // Whether there is ep
constexpr UCH EP_SQ = 63;   // Ep square

// Move metadata
constexpr UCH PROMO_YES = 4;
constexpr UCH PROMO = 3;

// Other
const char KNIGHT_OFFSETS[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {2, -1}, {2, 1}, {1, -2}, {1, 2},
};
const char KING_OFFSETS[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1},
};


// Helpers

/**
 * Get character representation of a piece.
 * piece2char(Position::WP) -> 'P'
 * @throws Errors::InvalidArg
 */
char piece2char(const char piece);

/**
 * Get piece for char representation.
 * char2piece('P') -> Position::WP
 * @throws Errors::InvalidArg
 */
char char2piece(const char piece);

/**
 * Get algebraic representation (a1, e4, h8) given square number.
 */
std::string sq2alg(const char sq);

/**
 * Get square from algebraic representation (a1, e4, h8).
 */
char alg2sq(const std::string alg);

/**
 * Get character repr of promotion for move e.g. Q, R, N, B
 */
char promo2char(const char promo);

/**
 * Get promotion for move e.g. 0, 1, 2, 3 from character repr
 */
char char2promo(const char ch);


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
     * * castling (4 bits): WK, WQ, BK, BQ
     * * turn (1 bit): true = white
     */
    UCH meta;

    /**
     * En passant metadata containing:
     * * EP square (6 bits)
     * * Is there EP (1 bit)
     */
    UCH ep;

    /**
     * Number of plies after the last capture or pawn move.
     */
    USH moveclock;

    /**
     * Number of moves. Starts at 1 and increments after black's move.
     */
    USH fullmoves;

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
     * Initialize with FEN string.
     * Make sure each section is separated by exactly 1 space.
     */
    Position(const std::string fen);

    /**
     * Return the piece at the position specified.
     * e.g. WP, BR, EMPTY
     */
    char get_at(const char sq) const;

    /**
     * Set position to piece.
     * Piece is WP, BR, EMPTY, ...
     * @throws Errors::InvalidArg
     */
    void set_at(const char sq, const char piece);

    /**
     * Get the FEN for the current position.
     */
    std::string fen() const;
};

/**
 * Move struct, containing start and end square.
 * Position is responsible for detecting castling and en passant.
 */
struct Move {
    /**
     * Squares from 0 to 63
     */
    UCH from, to;

    /**
     * Promotion:
     * * 2 bits: promo piece (N B R Q) in that order.
     * * 1 bit: whether it is a promotion
     */
    UCH promo;

    /**
     * No initialization.
     * Warning: Member variables may take arbitrary values.
     */
    Move();

    /**
     * Initialize with start and end square.
     * No promotion.
     */
    Move(const UCH from, const UCH to);

    /**
     * Initialize with start, end, and promo.
     * Yes promotion. @param promo 0 to 3.
     */
    Move(const UCH from, const UCH to, const UCH promo);

    /**
     * Get UCI representation of move.
     * e.g. e2e4, e7e8Q
     */
    std::string uci() const;
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
 * @param thru_king true = sliding pieces move through king (doesn't ignore king attacks though)
 */
ULL attacked(const Position& pos, const bool side, const bool thru_king = false);

/**
 * Generate a bitboard of opponent pieces checking the king (respect to @param side)
 */
ULL checkers(const Position& pos, const bool side, const UCH kpos, const UCH kx, const UCH ky);

/**
 * Generate legal moves and store in @param moves.
 */
void legal_moves(std::vector<Move>& moves, const Position& pos);


}  // namespace Position

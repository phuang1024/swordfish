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
 * Simple utils. All implementations are in this file.
 *
 * If it is a function, make it a #define or else there will be
 * multiple definition error.
 *
 * BE VERY CAREFUL WITH PARENTHESES! Keep in mind that an argument
 * may be an expression.
 */

#pragma once

typedef  unsigned char       UCH;
typedef  unsigned short      USH;
typedef  unsigned long long  ULL;


enum Errors {
    InvalidArg,
    AssertionError,
};

/**
 * Check if a bit is set.
 */
#define  bit(n, pos)  ((n) & (1ULL << (pos)))

/**
 * Check if a bit is not set. Removes ! in bool exprs.
 */
#define  nbit(n, pos)  (!((n) & (1ULL << (pos))))

/**
 * Set the bit. Does not modify in place.
 */
#define  bset(n, pos)  ((n) | (1ULL << (pos)))

/**
 * Unset the bit. Does not modify in place.
 */
#define  bunset(n, pos)  ((n) & ~(1ULL << (pos)))

/**
 * Check if location is in board (0 <= x < 8) and same for y.
 */
#define  in_board(x, y)  (0 <= (x) && (x) < 8 && 0 <= (y) && (y) < 8)

/**
 * Square value from 0 to 64 given x and y.
 */
#define  square(x, y)  ((x) + ((y) << 3))

/**
 * Bitboard of all pieces (pos.wp | pos.bp | ...)
 */
#define  all_pieces(p)  (p.wp|p.wn|p.wb|p.wr|p.wq|p.wk|p.bp|p.bn|p.bb|pos.br|p.bq|p.bk)

/**
 * Throw error if expression is false.
 */
#define  assert(expr)  {if (!(expr)) throw Errors::AssertionError;}

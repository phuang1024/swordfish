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

typedef  unsigned long long  ULL;
typedef  unsigned char       UCH;


enum Errors {
    InvalidArg,
    UnitTestFailed,
};

#define  bit(n, pos)     (n & (1ULL << (pos)))
#define  nbit(n, pos)    (!(n & (1ULL << (pos))))
#define  bset(n, pos)    (n | (1ULL << (pos)))
#define  bunset(n, pos)  (n & ~(1ULL << (pos)))

#define  in_board(x, y)  (0 <= (x) && (x) < 8 && 0 <= (y) && (y) < 8)
#define  square(x, y)    ((x) + ((y) << 3))

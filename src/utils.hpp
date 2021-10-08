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
 * Hopefully the compiler will inline them.
 */

#pragma once

typedef  unsigned long long  ULL;
typedef  unsigned char       UCH;


enum Errors {
    InvalidArg
};


/**
 * Check if a bit is set. Bit 0 is lowest bit.
 */
bool bit(const ULL num, const char bit_pos) {
    return num & (1ULL << bit_pos);
}

/**
 * Check if the bit is not set.
 * Cleans up '!'s in boolean expressions.
 */
bool nbit(const ULL num, const char bit_pos) {
    return !bit(num, bit_pos);
}

/**
 * Set the bit to 1.
 */
ULL bset(const ULL num, const char bit) {
    return num | (1ULL << bit);
}

/**
 * Set the bit to 0.
 */
ULL bunset(const ULL num, const char bit) {
    return num & ~(1ULL << bit);
}

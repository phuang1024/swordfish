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
 * Handles move generation.
 */

#include "position.hpp"


namespace Position {


ULL attacked(const Position& pos, const bool side) {
    ULL board = 0;

    for (int sq = 0; sq < 64; sq++) {
        const int x = sq & 7, y = sq >> 3;
        const char piece = pos.piece_at(sq);

        if ((bool)(piece & 8) != side)  // piece is wrong color
            continue;

        if (piece == EMPTY) {  // check this first so continue earlier.
            continue;
        } else if (piece == WP || piece == BP) {
            if (side && y < 7) {
                if (x > 0) board = bset(board, sq+7);
                if (x < 7) board = bset(board, sq+9);
            } else if (!side && y > 0) {
                if (x > 0) board = bset(board, sq-9);
                if (x < 7) board = bset(board, sq-7);
            }
        }
    }

    return board;
}


void legal_moves(std::vector<Move>& moves, const Position& pos) {
    moves.clear();
}


}  // namespace Position

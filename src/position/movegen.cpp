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


void attacked_pawn(const bool side, ULL& board, const int x, const int y, const int sq) {
    if (side && y < 7) {
        if (x > 0) board = bset(board, sq+7);
        if (x < 7) board = bset(board, sq+9);
    } else if (!side && y > 0) {
        if (x > 0) board = bset(board, sq-9);
        if (x < 7) board = bset(board, sq-7);
    }
}

void attacked_knight(ULL& board, const int x, const int y) {
    for (int i = 0; i < 8; i++) {
        const int cx = x + KNIGHT_OFFSETS[i][0], cy = y + KNIGHT_OFFSETS[i][1];
        if (in_board(cx, cy))
            board = bset(board, cx + (cy<<3));
    }
}

void attacked_sliding(ULL& board, int x, int y, const ULL pieces, const int dx, const int dy) {
    while (true) {
        x += dx;
        y += dy;
        if (!in_board(x, y))
            break;

        const char sq = square(x, y);
        board = bset(board, sq);
        if (bit(pieces, sq))
            break;
    }
}

void attacked_bishop(ULL& board, const int x, const int y, const ULL pieces) {
    attacked_sliding(board, x, y, pieces, 1, 1);
    attacked_sliding(board, x, y, pieces, 1, -1);
    attacked_sliding(board, x, y, pieces, -1, 1);
    attacked_sliding(board, x, y, pieces, -1, -1);
}

void attacked_rook(ULL& board, const int x, const int y, const ULL pieces) {
    attacked_sliding(board, x, y, pieces, 0, 1);
    attacked_sliding(board, x, y, pieces, 0, -1);
    attacked_sliding(board, x, y, pieces, 1, 0);
    attacked_sliding(board, x, y, pieces, -1, 0);
}

void attacked_queen(ULL& board, const int x, const int y, const ULL pieces) {
    attacked_bishop(board, x, y, pieces);
    attacked_rook(board, x, y, pieces);
}

void attacked_king(ULL& board, const int x, const int y) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0)
                continue;
            const int nx = x + dx, ny = y + dy;
            if (in_board(nx, ny))
                board = bset(board, square(x, y));
        }
    }
}

ULL attacked(const Position& pos, const bool side) {
    const ULL pieces = all_pieces(pos);

    ULL board = 0;

    for (int sq = 0; sq < 64; sq++) {
        const int x = sq & 7, y = sq >> 3;
        const char piece = pos.get_at(sq);

        if ((bool)(piece & 8) != side)  // piece is wrong color
            continue;

        if (piece == EMPTY)  // check this first so continue earlier.
            continue;
        else if (piece == WP || piece == BP)
            attacked_pawn(side, board, x, y, sq);
        else if (piece == WN || piece == BN)
            attacked_knight(board, x, y);
        else if (piece == WB || piece == BB)
            attacked_bishop(board, x, y, pieces);
        else if (piece == WR || piece == BR)
            attacked_rook(board, x, y, pieces);
        else if (piece == WQ || piece == BQ)
            attacked_queen(board, x, y, pieces);
        else if (piece == WK || piece == BK)
            attacked_king(board, x, y);
    }

    return board;
}


void legal_moves(std::vector<Move>& moves, const Position& pos) {
    moves.clear();
}


}  // namespace Position

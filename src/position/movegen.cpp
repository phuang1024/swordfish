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
    for (int i = 0; i < 8; i++) {
        const int cx = x + KING_OFFSETS[i][0], cy = y + KING_OFFSETS[i][1];
        if (in_board(cx, cy))
            board = bset(board, square(cx, cy));
    }
}

ULL attacked(const Position& pos, const bool side, const bool thru_king) {
    ULL pieces = all_pieces(pos);
    if (thru_king)
        pieces &= ~pos.wk & ~pos.bk;

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


ULL checkers(const Position& pos, const bool side, const UCH kpos, const UCH kx, const UCH ky,
             const RespectivePieces& rpieces) {
    ULL board = 0;  // pieces that are checking the king
    const ULL pieces = all_pieces(pos);

    // Pawns
    int pawn_offset = (side ? 1 : -1);   // y offset to find attacking pawns
    if ((side && ky < 7) || (!side && ky > 0)) {
        if (kx > 0) {
            UCH pos = square(kx-1, ky+pawn_offset);
            if (bit(rpieces.OP, pos))
                board = bset(board, pos);
        }
        if (kx < 7) {
            UCH pos = square(kx+1, ky+pawn_offset);
            if (bit(rpieces.OP, pos))
                board = bset(board, pos);
        }
    }

    // Knights
    for (int i = 0; i < 8; i++) {
        const int cx = kx + KNIGHT_OFFSETS[i][0], cy = ky + KNIGHT_OFFSETS[i][1];
        if (in_board(cx, cy)) {
            const UCH pos = square(cx, cy);
            if (bit(rpieces.ON, pos))
                board = bset(board, pos);
        }
    }

    return board;
}


void king_moves(std::vector<Move>& moves, const UCH kx, const UCH ky, const ULL attacks) {
    const int from = square(kx, ky);
    for (int i = 0; i < 8; i++) {
        const int cx = kx + KING_OFFSETS[i][0], cy = ky + KING_OFFSETS[i][1];
        const int sq = square(cx, cy);
        if (in_board(cx, cy) && nbit(attacks, sq))
            moves.push_back(Move(from, sq));
    }
}

void legal_moves(std::vector<Move>& moves, const Position& pos) {
    moves.clear();

    const bool side = pos.meta & TURN;

    // SP = same side pawns. ON = other side knights.
    ULL SP, SN, SB, SR, SQ, SK, OP, ON, OB, OR, OQ, OK;
    if (side) {  // yes this is bad style
        SP = pos.wp;  SN = pos.wn;  SB = pos.wb;  SR = pos.wr;  SQ = pos.wq;  SK = pos.wk;
        OP = pos.bp;  ON = pos.bn;  OB = pos.bb;  OR = pos.br;  OQ = pos.bq;  OK = pos.bk;
    } else {
        SP = pos.bp;  SN = pos.bn;  SB = pos.bb;  SR = pos.br;  SQ = pos.bq;  SK = pos.bk;
        OP = pos.wp;  ON = pos.wn;  OB = pos.wb;  OR = pos.wr;  OQ = pos.wq;  OK = pos.wk;
    }
    const RespectivePieces rpieces(SP, SN, SB, SR, SQ, SK, OP, ON, OB, OR, OQ, OK);

    const UCH kpos = bpos(side ? pos.wk : pos.bk);
    const UCH kx = kpos & 7, ky = kpos >> 3;

    const ULL o_attacks = attacked(pos, !side, true);

    ULL a = checkers(pos, side, kpos, kx, ky, rpieces);
    print(std::cout, a);
    //king_moves(moves, kx, ky, o_attacks);
}


}  // namespace Position

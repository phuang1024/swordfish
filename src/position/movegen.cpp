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


ULL bb_ray(const char sq1, const char sq2) {
    const int x1 = sq1 & 7, y1 = sq1 >> 3;
    const int x2 = sq2 & 7, y2 = sq2 >> 3;
    const int abs_dx = x2 - x1, abs_dy = y2 - y1;

    const int dx = (abs_dx == 0) ? 0 : (abs_dx > 0 ? 1 : -1);
    const int dy = (abs_dy == 0) ? 0 : (abs_dy > 0 ? 1 : -1);

    ULL board = 0;
    int x = x1, y = y1;
    while (true) {
        board = bset(board, square(x, y));
        if (!(x != x2 || y != y2))
            break;
        x += dx;
        y += dy;
    }

    return board;
}


/**
 * Generate squares of sliding piece and store in board.
 * Starts at (x, y). Moves with offset (dx, dy).
 * @param sp bitboard of same side pieces.
 * @param op bitboard of other side pieces.
 * @param inc_start whether to include starting square.
 * @param inc_end_sp whether to include ending square when hitting a same side piece.
 * @param inc_end_op whether to include ending square when hitting a other side piece.
 * @param inc_mid whether to include middle pieces.
 */
void bb_sliding(ULL& board, int x, int y, int dx, int dy, ULL sp, ULL op, bool inc_start,
                bool inc_end_sp, bool inc_end_op, bool inc_mid) {
    if (inc_start)
        board = bset(board, square(x, y));

    while (true) {
        x += dx;
        y += dy;
        if (!in_board(x, y))
            break;

        const char sq = square(x, y);
        if (bit(sp, sq)) {
            if (inc_end_sp)
                board = bset(board, sq);
            break;
        }
        if (bit(op, sq)) {
            if (inc_end_op)
                board = bset(board, sq);
            break;
        }

        if (inc_mid)
            board = bset(board, sq);
    }
}

ULL attacked(const Position& pos, ULL spieces, ULL opieces, bool side, bool thru_king) {
    if (thru_king) {
        spieces &= ~pos.wk & ~pos.bk;
        opieces &= ~pos.wk & ~pos.bk;
    }

    ULL board = 0;
    for (int sq = 0; sq < 64; sq++) {
        const char piece = pos.get_at(sq);
        if (piece == EMPTY)
            continue;
        if ((bool)(piece & 8) != side)  // piece is wrong color
            continue;

        const int x = sq & 7, y = sq >> 3;

        if (piece == WP || piece == BP) {
            if (side && y < 7) {
                if (x > 0) board = bset(board, sq+7);
                if (x < 7) board = bset(board, sq+9);
            } else if (!side && y > 0) {
                if (x > 0) board = bset(board, sq-9);
                if (x < 7) board = bset(board, sq-7);
            }
        }

        if (piece == WN || piece == BN) {
            for (int i = 0; i < 8; i++) {
                const int cx = x + KNIGHT_OFFSETS[i][0], cy = y + KNIGHT_OFFSETS[i][1];
                if (in_board(cx, cy))
                    board = bset(board, square(cx, cy));
            }
        }

        if (piece == WB || piece == BB || piece == WQ || piece == BQ) {
            for (int i = 0; i < 4; i++) {
                const int dx = BISHOP_OFFSETS[i][0], dy = BISHOP_OFFSETS[i][1];
                bb_sliding(board, x, y, dx, dy, spieces, opieces, false, true, true, true);
            }
        }

        if (piece == WR || piece == BR || piece == WQ || piece == BQ) {
            for (int i = 0; i < 4; i++) {
                const int dx = ROOK_OFFSETS[i][0], dy = ROOK_OFFSETS[i][1];
                bb_sliding(board, x, y, dx, dy, spieces, opieces, false, true, true, true);
            }
        }

        if (piece == WK || piece == BK) {
            for (int i = 0; i < 8; i++) {
                const int cx = x + KING_OFFSETS[i][0], cy = y + KING_OFFSETS[i][1];
                if (in_board(cx, cy))
                    board = bset(board, square(cx, cy));
            }
        }
    }

    return board;
}


ULL checkers(const Position& pos, bool side, UCH kx, UCH ky, const RespectivePieces& rpieces) {
    ULL board = 0;  // pieces that are checking the king

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

    // Bishops and Queens
    for (int i = 0; i < 4; i++) {
        const int dx = BISHOP_OFFSETS[i][0], dy = BISHOP_OFFSETS[i][1];
        int x = kx, y = ky;
        while (true) {
            x += dx;
            y += dy;
            if (!in_board(x, y))
                break;

            const UCH pos = square(x, y);
            if (bit(rpieces.OB, pos) || bit(rpieces.OQ, pos)) {
                board = bset(board, pos);
                break;
            }
            if (bit(rpieces.ALL, pos))
                break;
        }
    }

    // Rooks and Queens
    for (int i = 0; i < 4; i++) {
        const int dx = ROOK_OFFSETS[i][0], dy = ROOK_OFFSETS[i][1];
        int x = kx, y = ky;
        while (true) {
            x += dx;
            y += dy;
            if (!in_board(x, y))
                break;

            const UCH pos = square(x, y);
            if (bit(rpieces.OR, pos) || bit(rpieces.OQ, pos)) {
                board = bset(board, pos);
                break;
            }
            if (bit(rpieces.ALL, pos))
                break;
        }
    }

    return board;
}


/**
 * Possible king moves. Takes into account check.
 */
void king_moves(std::vector<Move>& moves, const UCH kx, const UCH ky, const ULL attacks) {
    const int from = square(kx, ky);
    for (int i = 0; i < 8; i++) {
        const int cx = kx + KING_OFFSETS[i][0], cy = ky + KING_OFFSETS[i][1];
        const int sq = square(cx, cy);
        if (in_board(cx, cy) && nbit(attacks, sq))
            moves.push_back(Move(from, sq));
    }
}

/**
 * Moves that are not king.
 * Takes into account pins, checks, and en passant, ...
 * But NOT castling.
 */
void non_king_moves(std::vector<Move>& moves, const Position& pos, bool side, const ULL spieces,
                    const ULL opieces, const ULL capture_mask, const ULL push_mask) {
    const int pawn_offset = (side ? 1 : -1);
    const int pawn_promo = (side ? 6 : 1);
    const int pawn_two_moves = (side ? 1 : 6);

    for (int sq = 0; sq < 64; sq++) {
        const UCH piece = pos.get_at(sq);
        if (piece == EMPTY)
            continue;
        if ((bool)(piece & 8) != side)  // piece is wrong color
            continue;

        const int x = sq & 7, y = sq >> 3;

    }
}

void legal_moves(std::vector<Move>& moves, const Position& pos) {
    moves.clear();

    const bool side = pos.meta & TURN;
    const RespectivePieces rpieces(pos);

    const UCH kpos = bpos(side ? pos.wk : pos.bk);
    const UCH kx = kpos & 7, ky = kpos >> 3;

    const ULL o_attacks = attacked(pos, rpieces.SAME, rpieces.OTHER, !side, true);
    const ULL checks = checkers(pos, side, kx, ky, rpieces);
    const int num_checks = popcnt(checks);

    king_moves(moves, kx, ky, o_attacks);

    if (num_checks < 2) {
        const char check_pos = (num_checks == 1 ? bpos(checks) : -1);
        ULL capture_mask = ALL, push_mask = ALL;

        if (num_checks == 1) {
            capture_mask = checks;
            if (bit(rpieces.OB|rpieces.OR|rpieces.OQ, check_pos)) {
                push_mask = bb_ray(kpos, check_pos);
                push_mask = bunset(bunset(push_mask, kpos), check_pos);
            } else {
                push_mask = 0;
            }
        }

        non_king_moves(moves, pos, side, rpieces.SAME, rpieces.OTHER, capture_mask, push_mask);
    }
}


}  // namespace Position

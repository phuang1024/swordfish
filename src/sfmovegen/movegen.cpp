#include <iostream>

#include "sfmovegen.hpp"

namespace Movegen {


static inline ull attacks_pawn(int x, int y, bool side) {
    if (y == 0 || y == 7)
        return 0;

    const int dy = (side ? 1 : -1);

    ull attacks = 0;
    if (x > 0)
        attacks = Bit::set(attacks, square(x-1, y+dy));
    if (x < 7)
        attacks = Bit::set(attacks, square(x+1, y+dy));

    return attacks;
}

/**
 * Knight or king.
 */
static inline ull attacks_offsets(int x, int y, const int offsets[8][2]) {
    ull attacks = 0;
    for (int i = 0; i < 8; i++) {
        const int cx = x + offsets[i][0];
        const int cy = y + offsets[i][1];
        if (in_board(cx, cy))
            attacks = Bit::set(attacks, square(cx, cy));
    }

    return attacks;
}

/**
 * Rook, bishop, or queen.
 */
static inline ull attacks_sliding(int x, int y, const int offsets[4][2], ull all_pieces) {
    const int start = square(x, y);
    all_pieces = Bit::unset(all_pieces, start);

    ull attacks = 0;
    for (int i = 0; i < 4; i++) {
        attacks |= bb_sequence(start, offsets[i][0], offsets[i][1], all_pieces, false, true);
    }

    return attacks;
}

void board_info(bool turn, const RelativeBB& relbb, ull& r_attacked, ull& r_checkers) {
    const ull m_pieces = *relbb.mp | *relbb.mn | *relbb.mb | *relbb.mr | *relbb.mq | *relbb.mk;
    const ull t_pieces_nok = *relbb.tp | *relbb.tn | *relbb.tb | *relbb.tr | *relbb.tq;
    const ull t_pieces = t_pieces_nok | *relbb.tk;
    const ull a_pieces_nok = m_pieces | t_pieces_nok;
    const ull a_pieces = m_pieces | t_pieces;

    for (int i = 0; i < 64; i++) {
        const int x = i % 8, y = i / 8;
        ull attacks = 0;

        if (Bit::get(*relbb.mp, i)) {
            attacks |= attacks_pawn(x, y, turn);
        } else if (Bit::get(*relbb.mn, i)) {
            attacks |= attacks_offsets(x, y, KNIGHT_OFFSETS);
        } else if (Bit::get(*relbb.mk, i)) {
            attacks |= attacks_offsets(x, y, KING_OFFSETS);
        }
        if (Bit::get(*relbb.mb, i) || Bit::get(*relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, BISHOP_OFFSETS, a_pieces_nok);
        }
        if (Bit::get(*relbb.mr, i) || Bit::get(*relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, ROOK_OFFSETS, a_pieces_nok);
        }

        r_attacked |= attacks;
        if (attacks & *relbb.tk)
            r_checkers = Bit::set(r_checkers, i);
    }
}


static inline void get_king_moves(int kx, int ky, ull danger, std::vector<Move>& r_moves) {
    const int start = square(kx, ky);
    for (int i = 0; i < 8; i++) {
        const int x = kx + KING_OFFSETS[i][0], y = ky + KING_OFFSETS[i][1];
        if (in_board(x, y)) {
            const int sq = square(x, y);
            if (!Bit::get(danger, sq))
                r_moves.push_back(Move(start, sq));
        }
    }
}

void get_legal_moves(Position& pos, std::vector<Move>& r_moves) {
    RelativeBB relbb = pos.relative_bb(pos.turn);
    ull attacked, checkers;
    board_info(!pos.turn, relbb.swap_sides(), attacked, checkers);
    const int num_checkers = Bit::popcnt(checkers);
    const int kpos = Bit::first(*relbb.mk);
    const int kx = kpos % 8, ky = kpos / 8;

    Ascii::print(std::cerr, attacked); std::cerr << std::endl;
    Ascii::print(std::cerr, checkers); std::cerr << std::endl;

    get_king_moves(kx, ky, attacked, r_moves);

    if (num_checkers >= 2) {
        // Only king moves.
        return;
    }

    ull capture_mask = 0xffffffffffffffff;
    ull push_mask = 0xffffffffffffffff;

    if (num_checkers == 1) {
        // Calculate push and capture mask.
        const int checker_pos = Bit::first(checkers);
        const int checker_x = checker_pos % 8, checker_y = checker_pos / 8;
        const int dx = (checker_x == kx ? 0 : (checker_x > kx ? 1 : -1)),
                  dy = (checker_y == ky ? 0 : (checker_y > ky ? 1 : -1));
        push_mask = bb_sequence(kpos, dx, dy, checkers, false, false);
        capture_mask = checkers;
    }

    // TODO generate other piece moves.
}


}  // namespace Movegen

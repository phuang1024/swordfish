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

    if (attacks != 0)
        std::cerr << attacks << std::endl;
    return attacks;
}

void board_info(const Position& pos, ull& r_attacked, ull& r_checkers) {
    RelativeBB relbb = pos.relative_bb(pos.turn);
    const ull m_pieces = relbb.mp | relbb.mn | relbb.mb | relbb.mr | relbb.mq | relbb.mk;
    const ull t_pieces = relbb.tp | relbb.tn | relbb.tb | relbb.tr | relbb.tq | relbb.tk;
    const ull a_pieces = m_pieces | t_pieces;
    const ull tk = relbb.tk;
    relbb.tk = 0;

    for (int i = 0; i < 64; i++) {
        const int x = i % 8, y = i / 8;
        ull attacks = 0;

        if (Bit::get(relbb.mp, i)) {
            attacks |= attacks_pawn(x, y, pos.turn);
        } else if (Bit::get(relbb.mn, i)) {
            attacks |= attacks_offsets(x, y, KNIGHT_OFFSETS);
        } else if (Bit::get(relbb.mk, i)) {
            attacks |= attacks_offsets(x, y, KING_OFFSETS);
        }
        if (Bit::get(relbb.mb, i) || Bit::get(relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, BISHOP_OFFSETS, a_pieces);
        }
        if (Bit::get(relbb.mr, i) || Bit::get(relbb.mq, i)) {
            attacks |= attacks_sliding(x, y, ROOK_OFFSETS, a_pieces);
        }

        r_attacked |= attacks;
        if (attacks & tk)
            r_checkers = Bit::set(r_checkers, i);
    }
}


}  // namespace Movegen

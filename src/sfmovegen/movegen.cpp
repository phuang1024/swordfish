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

void board_info(const Position& pos, ull& r_attacked, ull& r_checkers) {
    RelativeBB relbb = pos.relative_bb(pos.turn);
    const ull tk = relbb.tk;
    relbb.tk = 0;

    for (int i = 0; i < 64; i++) {
        const int x = i % 8, y = i / 8;
        ull attacks = 0;

        if (Bit::get(relbb.mp, i)) {
            attacks = attacks_pawn(x, y, pos.turn);
        } else if (Bit::get(relbb.mn, i)) {
            attacks = attacks_offsets(x, y, KNIGHT_OFFSETS);
        } else if (Bit::get(relbb.mk, i)) {
            attacks = attacks_offsets(x, y, KING_OFFSETS);
        }

        r_attacked |= attacks;
        if (attacks & tk)
            r_checkers = Bit::set(r_checkers, i);
    }
}


}  // namespace Movegen

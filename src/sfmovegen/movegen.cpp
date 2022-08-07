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

ull attacked_sqs(const Position& pos, bool side, bool ignore_king) {
    RelativeBB relbb = pos.relative_bb(side);
    ull attacks = 0;

    for (int i = 0; i < 64; i++) {
        const int x = i % 8, y = i / 8;

        if (Bit::get(relbb.mp, i))
            attacks |= attacks_pawn(x, y, side);
        else if (Bit::get(relbb.mn, i))
            attacks |= attacks_offsets(x, y, KNIGHT_OFFSETS);
        else if (Bit::get(relbb.mk, i))
            attacks |= attacks_offsets(x, y, KING_OFFSETS);
    }

    return attacks;
}


}  // namespace Movegen

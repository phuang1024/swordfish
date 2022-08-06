#pragma once

#include <iostream>

using ull = unsigned long long;


constexpr bool WHITE = true,
               BLACK = false;

// Piece codes.
constexpr int EMPTY = 0,
              WP = 1,
              WN = 2,
              WB = 3,
              WR = 4,
              WQ = 5,
              WK = 6,
              BP = 7,
              BN = 8,
              BB = 9,
              BR = 10,
              BQ = 11,
              BK = 12;


/**
 * Bit manipulation functions.
 */
namespace Bit {
    inline ull mask(int i) {
        return 1ULL << i;
    }

    inline bool get(ull b, int i) {
        return b & mask(i);
    }

    inline ull set(ull b, int i) {
        return b | mask(i);
    }

    inline ull unset(ull b, int i) {
        return b & ~mask(i);
    }
}


/**
 * Convert X, Y to square code.
 */
inline int square(int x, int y) {
    return x + 8 * y;
}


/**
 * Convert piece code (e.g. WP) into char representation (e.g. 'P')
 */
inline char piece_char(int piece) {
    switch (piece) {
        case WP: return 'P';
        case WN: return 'N';
        case WB: return 'B';
        case WR: return 'R';
        case WQ: return 'Q';
        case WK: return 'K';
        case BP: return 'p';
        case BN: return 'n';
        case BB: return 'b';
        case BR: return 'r';
        case BQ: return 'q';
        case BK: return 'k';
        default: return ' ';
    }
}


/**
 * Chess position using bitboards.
 * Square numbers: 0 = A1, 1 = A2, ..., 63 = H8.
 */
class Position {
public:
    ull wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;

    ~Position() {
    }

    Position() {
        wp = wn = wb = wr = wq = wk = bp = bn = bb = br = bq = bk = 0;
    }

    /**
     * Get piece code at position.
     */
    inline int piece_at(int pos) const {
        if (Bit::get(wp, pos)) return WP;
        if (Bit::get(wn, pos)) return WN;
        if (Bit::get(wb, pos)) return WB;
        if (Bit::get(wr, pos)) return WR;
        if (Bit::get(wq, pos)) return WQ;
        if (Bit::get(wk, pos)) return WK;
        if (Bit::get(bp, pos)) return BP;
        if (Bit::get(bn, pos)) return BN;
        if (Bit::get(bb, pos)) return BB;
        if (Bit::get(br, pos)) return BR;
        if (Bit::get(bq, pos)) return BQ;
        if (Bit::get(bk, pos)) return BK;
        return EMPTY;
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

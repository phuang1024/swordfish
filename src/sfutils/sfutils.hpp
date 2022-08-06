#pragma once

#include <iostream>
#include <string>

using ull = unsigned long long;


constexpr bool WHITE = true,
               BLACK = false;

// Piece codes.
constexpr int
    EMPTY = 0,
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


// Starting bitboards.
constexpr ull
    START_WP = 65280ULL,
    START_WN = 66ULL,
    START_WB = 36ULL,
    START_WR = 129ULL,
    START_WQ = 8ULL,
    START_WK = 16ULL,
    START_BP = 71776119061217280ULL,
    START_BN = 4755801206503243776ULL,
    START_BB = 2594073385365405696ULL,
    START_BR = 9295429630892703744ULL,
    START_BQ = 576460752303423488ULL,
    START_BK = 1152921504606846976ULL;


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
 * Convert between internal and human-readable notation.
 */
namespace Ascii {
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
     * Convert square code to string representation (e.g. "a1")
     */
    inline std::string square_str(int square) {
        char file = 'a' + (square % 8);
        char rank = '1' + (square / 8);
        return std::string(1, file) + std::string(1, rank);
    }
}


/**
 * Convert X, Y to square code.
 */
inline int square(int x, int y) {
    return x + 8 * y;
}


/**
 * Move from one square to next.
 */
class Move {
public:
    int from, to;

    Move(int from, int to) {
        this->from = from;
        this->to = to;
    }

    inline std::string uci() const {
        return Ascii::square_str(from) + Ascii::square_str(to);
    }

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << move.uci();
        return os;
    }
};


/**
 * Chess position using bitboards.
 * Square numbers: 0 = A1, 1 = A2, ..., 63 = H8.
 */
class Position {
public:
    ull wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;

    /**
     * All bitboards zero.
     */
    Position() {
        wp = wn = wb = wr = wq = wk = bp = bn = bb = br = bq = bk = 0;
    }

    /**
     * Standard starting chess position.
     */
    void setup_std() {
        wp = START_WP;
        wn = START_WN;
        wb = START_WB;
        wr = START_WR;
        wq = START_WQ;
        wk = START_WK;
        bp = START_BP;
        bn = START_BN;
        bb = START_BB;
        br = START_BR;
        bq = START_BQ;
        bk = START_BK;
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

    /**
     * Get ref to bitboard with piece at position.
     */
    inline ull& piece_bb(int pos) {
        if (Bit::get(wp, pos)) return wp;
        if (Bit::get(wn, pos)) return wn;
        if (Bit::get(wb, pos)) return wb;
        if (Bit::get(wr, pos)) return wr;
        if (Bit::get(wq, pos)) return wq;
        if (Bit::get(wk, pos)) return wk;
        if (Bit::get(bp, pos)) return bp;
        if (Bit::get(bn, pos)) return bn;
        if (Bit::get(bb, pos)) return bb;
        if (Bit::get(br, pos)) return br;
        if (Bit::get(bq, pos)) return bq;
        if (Bit::get(bk, pos)) return bk;
        std::cerr << "sfutils:Position:piece_bb: no piece at position " << pos << std::endl;
        throw 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

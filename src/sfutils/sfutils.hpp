#pragma once

#include <iostream>
#include <string>

using uch = unsigned char;
using ull = unsigned long long;

class Position;
class Move;


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

// Castling
constexpr int
    CASTLE_K = 1,
    CASTLE_Q = 2,
    CASTLE_k = 4,
    CASTLE_q = 8;

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
    inline char piece2char(int piece) {
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
     * Convert char (e.g. 'P') to piece (e.g. WP)
     */
    inline int char2piece(char ch) {
        switch (ch) {
            case 'P': return WP;
            case 'N': return WN;
            case 'B': return WB;
            case 'R': return WR;
            case 'Q': return WQ;
            case 'K': return WK;
            case 'p': return BP;
            case 'n': return BN;
            case 'b': return BB;
            case 'r': return BR;
            case 'q': return BQ;
            case 'k': return BK;
            default: return EMPTY;
        }
    }

    /**
     * Convert square code (e.g. 0) to string representation (e.g. "a1")
     */
    inline std::string square2str(int square) {
        char file = 'a' + (square % 8);
        char rank = '1' + (square / 8);
        return std::string(1, file) + std::string(1, rank);
    }

    /**
     * Convert square str (e.g. "a1") to square code (e.g. 0)
     */
    inline int str2square(std::string str) {
        return (str[1] - '1') * 8 + (str[0] - 'a');
    }

    /**
     * Print bitboard.
     */
    void print(std::ostream& os, ull bb);

    /**
     * Print board.
     */
    void print(std::ostream& os, const Position& pos);

    /**
     * Print move.
     */
    void print(std::ostream& os, const Move& move);
}


/**
 * Convert X, Y to square code.
 */
inline int square(int x, int y) {
    return x + 8 * y;
}

inline int in_board(int sq) {
    return 0 <= sq && sq < 64;
}

inline int in_board(int x, int y) {
    return (0 <= x && x < 8)
        && (0 <= y && y < 8);
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
        return Ascii::square2str(from) + Ascii::square2str(to);
    }
};


/**
 * Instead of white and black sides, stores bitboards of "my" and "their".
 * e.g. mk = my king, tp = their pawns.
 */
class RelativeBB {
public:
    ull mp, mn, mb, mr, mq, mk;
    ull tp, tn, tb, tr, tq, tk;
};


/**
 * Chess position using bitboards.
 * Square numbers: 0 = A1, 1 = A2, ..., 63 = H8.
 */
class Position {
public:
    ull wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;
    bool turn;
    uch castling;
    char ep;
    uch moves50;  // Fifty move rule.
    uch move;  // TODO this and above may have too small capacity.

    /**
     * NO initialization (may contain arbitrary values).
     * Use setup_std() to setup standard chess board.
     */
    Position() {
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
        turn = WHITE;
        castling = 15;
        ep = -1;
    }

    /**
     * Load position from FEN.
     */
    void setup_fen(std::string fen);

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
        std::cerr << "sfutils:Position:piece_bb: no piece at position: " << pos << std::endl;
        throw 0;
    }

    /**
     * Get relative BB.
     * @param my_side  WHITE or BLACK
     */
    inline RelativeBB relative_bb(bool my_side) const {
        RelativeBB relbb;
        if (my_side) {
            relbb.mp = wp;
            relbb.mn = wn;
            relbb.mb = wb;
            relbb.mr = wr;
            relbb.mq = wq;
            relbb.mk = wk;
            relbb.tp = bp;
            relbb.tn = bn;
            relbb.tb = bb;
            relbb.tr = br;
            relbb.tq = bq;
            relbb.tk = bk;
        } else {
            relbb.mp = bp;
            relbb.mn = bn;
            relbb.mb = bb;
            relbb.mr = br;
            relbb.mq = bq;
            relbb.mk = bk;
            relbb.tp = wp;
            relbb.tn = wn;
            relbb.tb = wb;
            relbb.tr = wr;
            relbb.tq = wq;
            relbb.tk = wk;
        }
        return relbb;
    }

    /**
     * Get FEN string for this position.
     */
    std::string get_fen() const;

    void set_at(int sq, int piece) {
        if (piece == WP) wp = Bit::set(wp, sq);
        else if (piece == WN) wn = Bit::set(wn, sq);
        else if (piece == WB) wb = Bit::set(wb, sq);
        else if (piece == WR) wr = Bit::set(wr, sq);
        else if (piece == WQ) wq = Bit::set(wq, sq);
        else if (piece == WK) wk = Bit::set(wk, sq);
        else if (piece == BP) bp = Bit::set(bp, sq);
        else if (piece == BN) bn = Bit::set(bn, sq);
        else if (piece == BB) bb = Bit::set(bb, sq);
        else if (piece == BR) br = Bit::set(br, sq);
        else if (piece == BQ) bq = Bit::set(bq, sq);
        else if (piece == BK) bk = Bit::set(bk, sq);
        else if (piece == EMPTY) {
            wp = Bit::unset(wp, sq);
            wn = Bit::unset(wn, sq);
            wb = Bit::unset(wb, sq);
            wr = Bit::unset(wr, sq);
            wq = Bit::unset(wq, sq);
            wk = Bit::unset(wk, sq);
            bp = Bit::unset(bp, sq);
            bn = Bit::unset(bn, sq);
            bb = Bit::unset(bb, sq);
            br = Bit::unset(br, sq);
            bq = Bit::unset(bq, sq);
            bk = Bit::unset(bk, sq);
        } else {
            std::cerr << "sfutils:Position:set_at: Invalid piece: " << piece << std::endl;
            throw 0;
        }
    }
};

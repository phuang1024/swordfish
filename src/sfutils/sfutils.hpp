#pragma once

#include <chrono>
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
    CASTLE_q = 8,
    CASTLE_W = CASTLE_K | CASTLE_Q,
    CASTLE_B = CASTLE_k | CASTLE_q;
constexpr ull
    CASTLE_SQS_K = 112ULL,
    CASTLE_SQS_Q = 28ULL,
    CASTLE_SQS_k = 8070450532247928832ULL,
    CASTLE_SQS_q = 2017612633061982208ULL;

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

enum Promo {
    NONE,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN
};


/**
 * Bit manipulation functions.
 */
namespace Bit {
    constexpr int TABLE_POPCNT[256] = {
        0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
    };

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

    inline int popcnt(ull b) {
        if (b == 0)
            return 0;

        int pop = 0;
        for (int i = 0; i < 8; i++) {
            pop += TABLE_POPCNT[b & 255];
            b >>= 8;
        }
        return pop;
    }

    /**
     * Assumes b has only one bit set.
     * Position of first bit set.
     * -1 if no bit set.
     */
    inline int first(ull b) {
        for (int i = 0; i < 8; i++) {
            int pos = -1;
            uch n = b & 255;
            switch (n) {
                case 1: pos = 0; break;
                case 2: pos = 1; break;
                case 4: pos = 2; break;
                case 8: pos = 3; break;
                case 16: pos = 4; break;
                case 32: pos = 5; break;
                case 64: pos = 6; break;
                case 128: pos = 7; break;
            }
            if (pos != -1)
                return pos + 8*i;

            b >>= 8;
        }
        return -1;
    }
}


namespace Time {
    /**
     * Milliseconds since epoch.
     */
    inline ull time() {
        const auto now = std::chrono::system_clock::now().time_since_epoch();
        const ull elapse = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
        return elapse;
    }

    /**
     * ms elapsed since arg
     */
    inline ull elapse(ull t) {
        return time() - t;
    }

    /**
     * Returns nodes per second.
     * 1e9 nps if elapse == 0
     * @param elapse  Milliseconds
     */
    inline ull nps(ull nodes, ull elapse) {
        if (elapse == 0)
            return (int)1e9;
        return nodes * 1000 / elapse;
    }
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
     * Convert promo code (e.g. KNIGHT) into char representation (e.g. 'N')
     */
    inline char promo2char(int promo) {
        switch (promo) {
            case Promo::KNIGHT: return 'N';
            case Promo::BISHOP: return 'B';
            case Promo::ROOK: return 'R';
            case Promo::QUEEN: return 'Q';
            default: return ' ';
        }
    }

    /**
     * Convert char promo (e.g. 'N') into promo code (e.g. KNIGHT)
     */
    inline int char2promo(char promo) {
        switch (promo) {
            case 'N': return Promo::KNIGHT;
            case 'B': return Promo::BISHOP;
            case 'R': return Promo::ROOK;
            case 'Q': return Promo::QUEEN;
            default: return -1;
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
        return square(str[0] - 'a', str[1] - '1');
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
 * Move from one square to next.
 */
class Move {
public:
    uch from, to;
    uch promo;

    Move(uch from, uch to, uch promo = Promo::NONE) {
        this->from = from;
        this->to = to;
        this->promo = promo;
    }

    Move(std::string uci) {
        from = Ascii::str2square(uci.substr(0, 2));
        to = Ascii::str2square(uci.substr(2, 2));
        if (uci.size() > 4)
            promo = Ascii::char2promo(uci[4]);
        else
            promo = Promo::NONE;
    }

    inline std::string uci() const {
        std::string str = Ascii::square2str(from) + Ascii::square2str(to);
        if (promo != Promo::NONE)
            str += Ascii::promo2char(promo);
        return str;
    }
};


/**
 * Instead of white and black sides, stores bitboards of "my" and "their".
 * e.g. mk = my king, tp = their pawns.
 */
class RelativeBB {
public:
    ull *mp, *mn, *mb, *mr, *mq, *mk;
    ull *tp, *tn, *tb, *tr, *tq, *tk;
    ull m_pieces, t_pieces, a_pieces;  // WARNING: Call init() before using.

    /**
     * Sets up m_pieces, t_pieces and a_pieces.
     */
    void init() {
        m_pieces = *mp | *mn | *mb | *mr | *mq | *mk;
        t_pieces = *tp | *tn | *tb | *tr | *tq | *tk;
        a_pieces = m_pieces | t_pieces;
    }

    RelativeBB swap_sides() {
        RelativeBB rbb;
        rbb.mp = tp; rbb.mn = tn; rbb.mb = tb; rbb.mr = tr; rbb.mq = tq; rbb.mk = tk;
        rbb.tp = mp; rbb.tn = mn; rbb.tb = mb; rbb.tr = mr; rbb.tq = mq; rbb.tk = mk;
        rbb.init();
        return rbb;
    }
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
     * Copy other position.
     */
    Position(const Position& other) {
        wp = other.wp;
        wn = other.wn;
        wb = other.wb;
        wr = other.wr;
        wq = other.wq;
        wk = other.wk;
        bp = other.bp;
        bn = other.bn;
        bb = other.bb;
        br = other.br;
        bq = other.bq;
        bk = other.bk;
        turn = other.turn;
        castling = other.castling;
        ep = other.ep;
        moves50 = other.moves50;
        move = other.move;
    }

    /**
     * Setup everything empty.
     */
    inline void setup_empty() {
        wp = wn = wb = wr = wq = wk = bp = bn = bb = br = bq = bk = 0;
        turn = false;
        castling = 0;
        ep = -1;
        moves50 = 0;
        move = 0;
    }

    /**
     * Standard starting chess position.
     */
    inline void setup_std() {
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
        moves50 = 0;
        move = 1;
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
     * @param my_side  WHITE or BLACK. Specifies which side is considered "mine".
     */
    inline RelativeBB relative_bb(bool my_side) {
        RelativeBB rbb;
        if (my_side) {
            rbb = (RelativeBB) {
                .mp = &wp, .mn = &wn, .mb = &wb, .mr = &wr, .mq = &wq, .mk = &wk,
                .tp = &bp, .tn = &bn, .tb = &bb, .tr = &br, .tq = &bq, .tk = &bk
            };
        } else {
            rbb = (RelativeBB) {
                .mp = &bp, .mn = &bn, .mb = &bb, .mr = &br, .mq = &bq, .mk = &bk,
                .tp = &wp, .tn = &wn, .tb = &wb, .tr = &wr, .tq = &wq, .tk = &wk
            };
        }
        rbb.init();
        return rbb;
    }

    /**
     * Get FEN string for this position.
     */
    std::string get_fen() const;

    /**
     * Doesn't clear other bbs first.
     * You can do that with set_at(sq, EMPTY); set_at(sq, your_choice);
     */
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

    /**
     * Play the move.
     * Assumes move is legal.
     * Otherwise, arbitrary behavior.
     */
    inline void push(const Move& m) {
        // TODO update move50

        // Erase m.to on all bitboards (capture).
        set_at(m.to, EMPTY);

        ull& bb = piece_bb(m.from);
        bb = Bit::unset(bb, m.from);
        if (m.promo == Promo::NONE) {
            // Normal move
            bb = Bit::set(bb, m.to);
        } else {
            // Promotion
            if (turn) {
                if (m.promo == Promo::KNIGHT) wn = Bit::set(wn, m.to);
                else if (m.promo == Promo::BISHOP) wb = Bit::set(wb, m.to);
                else if (m.promo == Promo::ROOK) wr = Bit::set(wr, m.to);
                else if (m.promo == Promo::QUEEN) wq = Bit::set(wq, m.to);
            } else {
                if (m.promo == Promo::KNIGHT) bn = Bit::set(bn, m.to);
                else if (m.promo == Promo::BISHOP) bb = Bit::set(bb, m.to);
                else if (m.promo == Promo::ROOK) br = Bit::set(br, m.to);
                else if (m.promo == Promo::QUEEN) bq = Bit::set(bq, m.to);
            }
        }

        // Castling.
        if (&bb == &wk) {
            if (m.from == square(4, 0)) {
                if (m.to == square(6, 0))
                    wr = Bit::set(Bit::unset(wr, square(7, 0)), square(5, 0));
                else if (m.to == square(2, 0))
                    wr = Bit::set(Bit::unset(wr, square(0, 0)), square(3, 0));
            }
            castling &= ~CASTLE_W;
        } else if (&bb == &bk) {
            if (m.from == square(4, 7)) {
                if (m.to == square(6, 7))
                    br = Bit::set(Bit::unset(br, square(7, 7)), square(5, 7));
                else if (m.to == square(2, 7))
                    br = Bit::set(Bit::unset(br, square(0, 7)), square(3, 7));
            }
            castling &= ~CASTLE_B;
        }
        if (&bb == &wr) {
            if (m.from == square(0, 0)) castling &= ~CASTLE_Q;
            else if (m.from == square(7, 0)) castling &= ~CASTLE_K;
        } else if (&bb == &br) {
            if (m.from == square(0, 7)) castling &= ~CASTLE_q;
            else if (m.from == square(7, 7)) castling &= ~CASTLE_k;
        }

        // EP capture.
        if ((&bb == &wp || &bb == &bp) && m.to == ep) {
            if (turn) {
                bp = Bit::unset(bp, m.to - 8);
            } else {
                wp = Bit::unset(wp, m.to + 8);
            }
        }

        // Change EP square.
        bool coords_correct = (m.from % 8 == m.to % 8) && abs(m.from / 8 - m.to / 8) == 2;
        if (coords_correct) {
            if (turn) ep = m.to - 8;
            else ep = m.to + 8;
        } else {
            ep = -1;
        }

        // Change turn.
        turn = !turn;
        if (turn)
            move++;
    }
};

#include "sfeval.hpp"


namespace Eval {


// Piece maps generated by copilot
static constexpr int MAP_PAWN[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};
static constexpr int MAP_KNIGHT[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};
static constexpr int MAP_BISHOP[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};
static constexpr int MAP_ROOK[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};
static constexpr int MAP_QUEEN[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
static constexpr int MAP_KING[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};
static constexpr int MAP_KING_EG[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};


/**
 * Checks if the game finished (checkmate, stalemate, draw).
 * @param attacks  Opposite turn's attacks.
 * @param kpos  Current turn's king pos.
 * @return  Large negative if black wins, large positive if white wins, 0 if draw.
 */
static inline int check_eog(bool turn, int move_count, ull attacks, int kpos, int mydepth) {
    if (move_count == 0) {
        if (!Bit::get(attacks, kpos))
            return 0;

        if (turn)
            return -MATE_SCORE + mydepth;
        else
            return MATE_SCORE - mydepth;
    }
    return 123456789;   // No eog constant.
}

static inline void material(const Position& pos, int& r_white, int& r_black) {
    r_white = r_black = 0;
    r_white += Bit::popcnt(pos.wp);
    r_black += Bit::popcnt(pos.bp);
    r_white += 3 * Bit::popcnt(pos.wn);
    r_black += 3 * Bit::popcnt(pos.bn);
    r_white += 3 * Bit::popcnt(pos.wb);
    r_black += 3 * Bit::popcnt(pos.bb);
    r_white += 5 * Bit::popcnt(pos.wr);
    r_black += 5 * Bit::popcnt(pos.br);
    r_white += 9 * Bit::popcnt(pos.wq);
    r_black += 9 * Bit::popcnt(pos.bq);
}

/**
 * Opening and middlegame policy.
 */
static inline int piece_map(const Position& pos, int phase) {
    int pawns, knights, bishops, rooks, queens, kings;
    pawns = knights = bishops = rooks = queens = kings = 0;

    for (int i = 0; i < 64; i++) {
        const int piece = pos.piece_at(i);
        if (piece == EMPTY)
            continue;

        const int sq = piece <= WK ? 63-i : i;  // Piece maps are reversed.
        const int mult = piece <= WK ? 1 : -1;
        switch (piece) {
            case WP: case BP: pawns += mult * MAP_PAWN[sq]; break;
            case WN: case BN: knights += mult * MAP_KNIGHT[sq]; break;
            case WB: case BB: bishops += mult * MAP_BISHOP[sq]; break;
            case WR: case BR: rooks += mult * MAP_ROOK[sq]; break;
            case WQ: case BQ: queens += mult * MAP_QUEEN[sq]; break;
            case WK: case BK:
                kings += mult * ((100-phase) * MAP_KING[sq]
                                 + phase * MAP_KING_EG[sq]) / 100;
                break;
        }
    }

    return (
        1.3 * pawns +
        0.8 * knights +
        0.9 * bishops +
        1.2 * rooks +
        1.3 * queens +
        1 * kings
    );
}

static inline int pawn_structure(ull wp, ull bp) {
    // Stacked pawns.
    int w_stacked = 0, b_stacked = 0;
    for (int x = 0; x < 8; x++) {
        int w_count = Bit::popcnt(wp & FILES[x]),
            b_count = Bit::popcnt(bp & FILES[x]);
        if (w_count > 0)
            w_stacked += w_count - 1;
        if (b_count > 0)
            b_stacked += b_count - 1;
    }

    return (
        (b_stacked - w_stacked)
    );
}

int eval(const Position& pos, int move_count, ull attacks, int kpos, int mydepth) {
    const int eog = check_eog(pos.turn, move_count, attacks, kpos, mydepth);
    if (eog != 123456789)
        return eog;

    int mat_w, mat_b;
    material(pos, mat_w, mat_b);
    int mat_total = mat_w + mat_b;
    int mat_score = 100 * (mat_w-mat_b);

    const int phase = std::min(std::max(-5*mat_total + 250, 0), 100);
    const int pm = piece_map(pos, phase);
    //const int pawns = pawn_structure(pos.wp, pos.bp);

    const int score = mat_score + 0.1*pm;// + pawns;
    return score;
}


}

#include "sfeval.hpp"


namespace Eval {


/**
 * Checks if the game finished (checkmate, stalemate, draw).
 * @return  Large negative if black wins, large positive if white wins, 0 if draw.
 */
static inline int check_eog(bool turn, int move_count) {
    if (move_count == 0) {
        if (turn)
            return -MATE_SCORE;
        else
            return MATE_SCORE;
    }
    // TODO stalemate check
    return 123456789;   // No eog code.
}

static inline int material(const Position& pos) {
    int score = 0;
    score += 100 * (Bit::popcnt(pos.wp) - Bit::popcnt(pos.bp));
    score += 300 * (Bit::popcnt(pos.wn) - Bit::popcnt(pos.bn));
    score += 300 * (Bit::popcnt(pos.wb) - Bit::popcnt(pos.bb));
    score += 500 * (Bit::popcnt(pos.wr) - Bit::popcnt(pos.br));
    score += 900 * (Bit::popcnt(pos.wq) - Bit::popcnt(pos.bq));
    return score;
}

int eval(const Position& pos, int move_count) {
    const int eog = check_eog(pos.turn, move_count);
    if (eog != 123456789)
        return eog;

    return material(pos);
}

int eval_rel(const Position& pos, int move_count) {
    const int score = eval(pos, move_count);
    return pos.turn ? score : -score;
}


}

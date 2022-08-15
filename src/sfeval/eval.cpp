#include "sfeval.hpp"


namespace Eval {


/**
 * Checks if the game finished (checkmate, stalemate, draw).
 * @return  Large negative if black wins, large positive if white wins, 0 if draw.
 */
static inline int check_eog(bool turn, int move_count) {
    if (move_count == 0) {
    }
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

int eval(const Position& pos) {
    return material(pos);
}


}

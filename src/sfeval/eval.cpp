#include "sfeval.hpp"


namespace Eval {


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

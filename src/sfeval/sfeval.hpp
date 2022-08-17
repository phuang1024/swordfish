#include "sfutils.hpp"


namespace Eval {
    constexpr int MATE_SCORE = 1e6;

    /**
     * Evaluation in centipawns from white's pov.
     */
    int eval(const Position& pos, int move_count);

    /**
     * Eval relative to position's side (positive is good).
     */
    int eval_rel(const Position& pos, int move_count);
}

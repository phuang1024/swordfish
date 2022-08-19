#include "sfutils.hpp"


namespace Eval {
    constexpr int MATE_SCORE = 1e6;

    /**
     * Evaluation in centipawns from current turn's pov.
     */
    int eval(const Position& pos, int move_count, ull attacks, int kpos, int mydepth);
}

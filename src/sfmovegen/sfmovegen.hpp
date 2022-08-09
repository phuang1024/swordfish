#pragma once

#include "sfutils.hpp"


/**
 * Generate legal moves of a chess position.
 */
namespace Movegen {
    const int KING_OFFSETS[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    const int KNIGHT_OFFSETS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, -2},
        {-2, 1}, {-2, -1}, {2, 1}, {2, -1}};

    /**
     * Set a sequence of squares.
     * Starts from start, increments by (dx, dy).
     * Stops at edge of board.
     * Also stops if corresponding bit is set in stop_bb.
     */
    inline ull bb_sequence(int start, int dx, int dy, ull stop_bb, bool include_stop) {
        const int delta = dx + 8*dy;

        ull bb = 0;
        int sq = start;
        while (in_board(sq)) {
            bool stop_here = Bit::get(stop_bb, sq);
            if (!include_stop && stop_here)
                break;
            bb = Bit::set(bb, sq);
            sq += delta;
            if (stop_here)
                break;
        }

        return bb;
    }

    /**
     * Get some info stored in return args.
     * @param r_attacked  Attacked squares ignoring enemy king.
     * @param r_checkers  Checkers.
     */
    void board_info(const Position& pos, ull& r_attacked, ull& r_checkers);
}

#pragma once

#include "sfutils.hpp"


/**
 * Generate legal moves of a chess position.
 */
namespace Movegen {
    const int KING_OFFSETS[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    const int KNIGHT_OFFSETS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, 2},
        {-2, 1}, {-2, -1}, {2, 1}, {2, -1}};

    /**
     * Set a sequence of squares.
     * Starts from start, increments by (dx, dy).
     * Stops at edge of board.
     */
    inline ull bb_sequence(int start, int dx, int dy) {
        const int delta = dx + 8*dy;

        ull bb = 0;
        int sq = start;
        while (in_board(sq)) {
            bb = Bit::set(bb, sq);
            sq += delta;
        }

        return bb;
    }

    /**
     * Get squares attacked by this side's pieces.
     * @param ignore_king  Whether the pieces see through the opponent's king.
     */
    ull attacked_sqs(const Position& pos, bool side, bool ignore_king);
}

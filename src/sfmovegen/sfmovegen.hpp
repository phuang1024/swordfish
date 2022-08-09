#pragma once

#include <vector>

#include "sfutils.hpp"


/**
 * Generate legal moves of a chess position.
 */
namespace Movegen {
    const int KING_OFFSETS[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    const int KNIGHT_OFFSETS[8][2] = {{-1, 2}, {1, 2}, {-1, -2}, {1, -2},
        {-2, 1}, {-2, -1}, {2, 1}, {2, -1}};
    const int BISHOP_OFFSETS[4][2] = {{-1, -1}, {1, 1}, {1, -1}, {-1, 1}};
    const int ROOK_OFFSETS[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    /**
     * Set a sequence of squares.
     * Starts from start, increments by (dx, dy).
     * Stops at edge of board.
     * Also stops if corresponding bit is set in stop_bb.
     */
    inline ull bb_sequence(int start, int dx, int dy, ull stop_bb,
            bool include_start, bool include_stop) {
        ull bb = 0;
        int x = start % 8, y = start / 8;
        bool is_start = true;
        while (in_board(x, y)) {
            const int sq = square(x, y);
            const bool stop_here = Bit::get(stop_bb, sq);
            if (!include_stop && stop_here)
                break;

            if (is_start && !include_start)
                is_start = false;
            else
                bb = Bit::set(bb, sq);
            x += dx;
            y += dy;

            if (stop_here)
                break;
        }

        return bb;
    }

    /**
     * Get some info stored in return args.
     * @param r_attacked  Attacked squares ignoring enemy king.
     * @param r_pinned  Enemy pieces pinned to enemy king.
     */
    void board_info(bool turn, const RelativeBB& relbb, ull& r_attacked, ull& r_checkers,
            ull& r_pinned);

    /**
     * Get all legal moves.
     * Appends moves to r_moves
     */
    void get_legal_moves(Position& pos, std::vector<Move>& r_moves);
}

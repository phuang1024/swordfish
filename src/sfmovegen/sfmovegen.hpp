#pragma once

#include "sfutils.hpp"


/**
 * Generate legal moves of a chess position.
 */
namespace Movegen {
    /**
     * Get squares attacked by this side's pieces.
     * @param ignore_king  Whether the pieces see through the opponent's king.
     */
    ull attacked_sqs(const Position& pos, bool side, bool ignore_king);
}

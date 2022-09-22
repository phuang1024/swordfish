#pragma once

#include <iostream>
#include <map>
#include <string>

#include "transposition.hpp"

#include "sfuci.hpp"
#include "sfutils.hpp"


/**
 * Move generation performance test.
 */
namespace Search {
    /**
     * nodes: Number of leaf nodes.
     */
    SearchResult perft(Position& pos, int depth);

    /**
     * Minimax.
     * pv: Bestmove.
     */
    Move search(Position& pos, int maxdepth, int movetime);

    /**
     * Computes move time from UCI args, e.g. wtime
     */
    int get_movetime(const Position& pos, std::map<std::string, int>& args);
}
